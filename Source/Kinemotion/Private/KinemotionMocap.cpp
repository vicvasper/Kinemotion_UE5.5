// Copyright (c) Victor Rivas Perez. All Rights Reserved.

#include "KinemotionMocap.h"

#include "Async/Async.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "Features/IModularFeatures.h"
#include "ILiveLinkClient.h"
#include "Kinemotion.h"
#include "KinemotionTypes.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "Misc/MediaBlueprintFunctionLibrary.h"
#include "Misc/Timecode.h"
#include "NNEModelData.h"
#include "RHI.h"
#include "RHICommandList.h"
#include "RenderingThread.h"
#include "SceneTypes.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkAnimationTypes.h"
#include "TimerManager.h"

namespace
{
	/**
	 * The rig published to Live Link.
	 *
	 * Bone names, the parent hierarchy and the mapping from tracked points onto bones used to
	 * be three separate hand-maintained lists - two arrays in the static-data setup and a run
	 * of per-bone calls in the frame path. Any edit had to be mirrored in all three, in
	 * matching index order, with nothing to catch a mismatch. They are one table now.
	 *
	 * ReferencePoint is the point each bone's translation is expressed relative to, which is
	 * not always its parent bone's driving point.
	 */
	struct FKinemotionBoneDef
	{
		const TCHAR* Name;
		int32 ParentBoneIndex;
		EKinemotionPoint Point;
		EKinemotionPoint ReferencePoint;
	};

	constexpr FKinemotionBoneDef KinemotionRig[] =
	{
		{ TEXT("root"),       -1, EKinemotionPoint::HipCenter,     EKinemotionPoint::HipCenter     },
		{ TEXT("pelvis"),      0, EKinemotionPoint::HipCenter,     EKinemotionPoint::HipCenter     },
		{ TEXT("spine_01"),    1, EKinemotionPoint::SpineMid,      EKinemotionPoint::HipCenter     },
		{ TEXT("neck_01"),     2, EKinemotionPoint::NeckBase,      EKinemotionPoint::SpineMid      },
		{ TEXT("head"),        3, EKinemotionPoint::Nose,          EKinemotionPoint::NeckBase      },

		{ TEXT("clavicle_l"),  3, EKinemotionPoint::LeftClavicle,  EKinemotionPoint::NeckBase      },
		{ TEXT("upperarm_l"),  5, EKinemotionPoint::LeftShoulder,  EKinemotionPoint::LeftClavicle  },
		{ TEXT("lowerarm_l"),  6, EKinemotionPoint::LeftElbow,     EKinemotionPoint::LeftShoulder  },
		{ TEXT("hand_l"),      7, EKinemotionPoint::LeftWrist,     EKinemotionPoint::LeftElbow     },

		{ TEXT("clavicle_r"),  3, EKinemotionPoint::RightClavicle, EKinemotionPoint::NeckBase      },
		{ TEXT("upperarm_r"),  9, EKinemotionPoint::RightShoulder, EKinemotionPoint::RightClavicle },
		{ TEXT("lowerarm_r"), 10, EKinemotionPoint::RightElbow,    EKinemotionPoint::RightShoulder },
		{ TEXT("hand_r"),     11, EKinemotionPoint::RightWrist,    EKinemotionPoint::RightElbow    },

		{ TEXT("thigh_l"),     1, EKinemotionPoint::LeftHip,       EKinemotionPoint::HipCenter     },
		{ TEXT("calf_l"),     13, EKinemotionPoint::LeftKnee,      EKinemotionPoint::LeftHip       },
		{ TEXT("foot_l"),     14, EKinemotionPoint::LeftAnkle,     EKinemotionPoint::LeftKnee      },

		{ TEXT("thigh_r"),     1, EKinemotionPoint::RightHip,      EKinemotionPoint::HipCenter     },
		{ TEXT("calf_r"),     16, EKinemotionPoint::RightKnee,     EKinemotionPoint::RightHip      },
		{ TEXT("foot_r"),     17, EKinemotionPoint::RightAnkle,    EKinemotionPoint::RightKnee     },
	};

	constexpr int32 KinemotionBoneCount = UE_ARRAY_COUNT(KinemotionRig);

	/** Root and pelvis are anchored rather than driven point-to-point, so they are handled apart. */
	constexpr int32 RootBoneIndex = 0;
	constexpr int32 PelvisBoneIndex = 1;
	constexpr int32 FirstDrivenBoneIndex = 2;

	/** Which model keypoint feeds each directly-tracked point. Derived points are absent. */
	struct FKinemotionKeypointMapping
	{
		EKinemotionPoint Point;
		int32 ModelIndex;
	};

	constexpr FKinemotionKeypointMapping KinemotionDecodedPoints[] =
	{
		{ EKinemotionPoint::Nose,           KinemotionKeypoint::Nose          },
		{ EKinemotionPoint::LeftShoulder,   KinemotionKeypoint::LeftShoulder  },
		{ EKinemotionPoint::RightShoulder,  KinemotionKeypoint::RightShoulder },
		{ EKinemotionPoint::LeftElbow,      KinemotionKeypoint::LeftElbow     },
		{ EKinemotionPoint::RightElbow,     KinemotionKeypoint::RightElbow    },
		{ EKinemotionPoint::LeftWrist,      KinemotionKeypoint::LeftWrist     },
		{ EKinemotionPoint::RightWrist,     KinemotionKeypoint::RightWrist    },
		{ EKinemotionPoint::LeftHip,        KinemotionKeypoint::LeftHip       },
		{ EKinemotionPoint::RightHip,       KinemotionKeypoint::RightHip      },
		{ EKinemotionPoint::LeftKnee,       KinemotionKeypoint::LeftKnee      },
		{ EKinemotionPoint::RightKnee,      KinemotionKeypoint::RightKnee     },
		{ EKinemotionPoint::LeftAnkle,      KinemotionKeypoint::LeftAnkle     },
		{ EKinemotionPoint::RightAnkle,     KinemotionKeypoint::RightAnkle    },
	};

	// Class defaults

	const TCHAR* const DefaultSubjectName = TEXT("KinemotionWebcam");
	const TCHAR* const DefaultPoseModelPath = TEXT("/Kinemotion/NeuralNetworks/pose_landmark_full.pose_landmark_full");
	const FName DefaultNneRuntimeName(TEXT("NNERuntimeORTDml"));

	constexpr float DefaultStickmanScale = 1.0f;
	constexpr float DefaultReferenceSizeCM = 170.0f;
	constexpr float DefaultClavicleBlend = 0.3f;
	constexpr float DefaultYawCorrectionDegrees = 90.0f;
	constexpr int32 DefaultLiveLinkFrameRate = 60;

	constexpr float DefaultSmoothingAlpha = 0.3f;
	constexpr float DefaultDeadzoneCM = 0.5f;
	constexpr float DefaultMaxStepCM = 15.0f;

	constexpr float DefaultDebugPointRadius = 5.0f;
	constexpr float DefaultDebugBoneThickness = 2.0f;

	/** Just over one frame at 60 Hz: long enough to stay visible, short enough not to pile up. */
	constexpr float DefaultDebugDrawLifetime = 0.05f;

	// Implementation constants

	/** Below this the smoothing filter is a no-op, so it is bypassed rather than run. */
	constexpr float MinEffectiveSmoothingAlpha = 0.01f;

	/** Midpoint weight for deriving centre points from a symmetric pair. */
	constexpr float SymmetricPairMidpoint = 0.5f;

	/** Passed to the capture-device enumerator to request every device, unfiltered. */
	constexpr int32 AllCaptureDeviceFilter = -1;

	/** Segments used for debug spheres. Low: these are diagnostics, drawn once per point per frame. */
	constexpr int32 DebugSphereSegments = 8;

	/** Bounded so a mesh that never populates its transforms cannot retry forever. */
	constexpr int32 MaxCalibrationAttempts = 30;

	const FName PelvisBoneName(TEXT("pelvis"));
	const FName NeckBoneName(TEXT("neck_01"));

	/**
	 * Decodes one SimCC axis into a normalised [0, 1] coordinate.
	 *
	 * The model classifies each axis into BinCount bins rather than regressing a coordinate,
	 * so the prediction is the index of the highest-scoring bin, normalised by the bin count.
	 */
	float DecodeSimccAxis(const float* Buffer, int32 BinCount)
	{
		checkSlow(Buffer != nullptr && BinCount > 0);

		float BestScore = -FLT_MAX;
		int32 BestBin = 0;

		for (int32 Bin = 0; Bin < BinCount; ++Bin)
		{
			if (Buffer[Bin] > BestScore)
			{
				BestScore = Buffer[Bin];
				BestBin = Bin;
			}
		}

		return static_cast<float>(BestBin) / static_cast<float>(BinCount);
	}
}

UKinemotionMocap::UKinemotionMocap()
	: SubjectName(DefaultSubjectName)
	, CameraIndex(0)
	, PoseModel(FSoftObjectPath(DefaultPoseModelPath))
	, NneRuntimeName(DefaultNneRuntimeName)
	, StickmanScale(DefaultStickmanScale)
	, bScaleRelativeToSkeleton(true)
	, ReferenceSizeCM(DefaultReferenceSizeCM)
	, bAutoCalibrate(true)
	, bUseFloorAsRoot(true)
	, bPelvisFree(false)
	, FloorZOffset(0.0f)
	, ClavicleBlend(DefaultClavicleBlend)
	, YawCorrectionDegrees(DefaultYawCorrectionDegrees)
	, LiveLinkFrameRate(DefaultLiveLinkFrameRate)
	, bEnableSmoothing(true)
	, SmoothingAlpha(DefaultSmoothingAlpha)
	, DeadzoneCM(DefaultDeadzoneCM)
	, MaxStepCM(DefaultMaxStepCM)
	, bShowDebug(false)
	, DebugPointRadius(DefaultDebugPointRadius)
	, DebugBoneThickness(DefaultDebugBoneThickness)
	, DebugDrawLifetime(DefaultDebugDrawLifetime)
{
	PrimaryComponentTick.bCanEverTick = true;

	PoseLocal.SetNumZeroed(KinemotionPointCount);
	PrevFilteredLocal.SetNumZeroed(KinemotionPointCount);
	bHasFilteredSample.Init(false, KinemotionPointCount);
}

void UKinemotionMocap::BeginPlay()
{
	Super::BeginPlay();

	SetupLiveLinkDirect();
	InitNNE();

	if (bAutoCalibrate)
	{
		CalibrateFromSkeleton();
	}

	InitMedia();
}

void UKinemotionMocap::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bPelvisInitialized = false;
	InitialPelvisWorld = FVector::ZeroVector;

	if (MediaPlayer)
	{
		MediaPlayer->Close();
	}

	ShutdownLiveLink();

	// Released before the component goes away so no inference can be in flight against a
	// model instance whose owner is being torn down.
	ModelInstance.Reset();

	Super::EndPlay(EndPlayReason);
}

void UKinemotionMocap::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// One read in flight at a time: the capture rate is bounded by inference, not by the tick.
	if (!bIsReadingFrame.load(std::memory_order_acquire) && ModelInstance.IsValid() && MediaTexture)
	{
		RequestTextureRead();
	}
}

// Live Link

void UKinemotionMocap::SetupLiveLinkDirect()
{
	if (!IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		UE_LOG(LogKinemotion, Error, TEXT("Live Link plugin is not enabled; capture cannot be published."));
		return;
	}

	ILiveLinkClient& LiveLinkClient =
		IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);

	// The source has to be registered before any subject data is pushed: without it the
	// subject exists but has no entry in the Live Link panel, so it cannot be inspected or
	// shut down from the UI. AddSource assigns the GUID and calls back into ReceiveClient.
	LiveLinkSource = MakeShared<FKinemotionLiveLinkSource>(FText::FromString(TEXT("Kinemotion Mocap")));
	SourceGuid = LiveLinkClient.AddSource(LiveLinkSource);

	SubjectKey = FLiveLinkSubjectKey(SourceGuid, FName(*SubjectName));

	FLiveLinkStaticDataStruct StaticData(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData* SkeletonData = StaticData.Cast<FLiveLinkSkeletonStaticData>();

	SkeletonData->BoneNames.Reserve(KinemotionBoneCount);
	SkeletonData->BoneParents.Reserve(KinemotionBoneCount);

	for (const FKinemotionBoneDef& Bone : KinemotionRig)
	{
		SkeletonData->BoneNames.Add(FName(Bone.Name));
		SkeletonData->BoneParents.Add(Bone.ParentBoneIndex);
	}

	LiveLinkClient.PushSubjectStaticData_AnyThread(
		SubjectKey, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));

	bLiveLinkRegistered = true;

	UE_LOG(LogKinemotion, Log, TEXT("Live Link source and subject registered as '%s' (%d bones)."),
		*SubjectName, KinemotionBoneCount);
}

void UKinemotionMocap::ShutdownLiveLink()
{
	if (!bLiveLinkRegistered)
	{
		return;
	}

	if (IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		ILiveLinkClient& LiveLinkClient =
			IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);

		LiveLinkClient.RemoveSubject_AnyThread(SubjectKey);

		// Removing only the subject leaves the source behind as a dead entry in the Live Link
		// panel, which accumulates across play sessions.
		if (SourceGuid.IsValid())
		{
			LiveLinkClient.RemoveSource(SourceGuid);
		}
	}

	LiveLinkSource.Reset();
	SourceGuid.Invalidate();
	bLiveLinkRegistered = false;
}

// Setup

void UKinemotionMocap::InitNNE()
{
	ModelData = PoseModel.LoadSynchronous();
	if (!ModelData)
	{
		UE_LOG(LogKinemotion, Error,
			TEXT("Pose model could not be loaded from '%s'. Check that plugin content is enabled."),
			*PoseModel.ToString());
		return;
	}

	TWeakInterfacePtr<INNERuntimeGPU> Runtime = UE::NNE::GetRuntime<INNERuntimeGPU>(NneRuntimeName.ToString());
	if (!Runtime.IsValid())
	{
		UE_LOG(LogKinemotion, Error, TEXT("NNE runtime '%s' is not available."), *NneRuntimeName.ToString());
		return;
	}

	TSharedPtr<UE::NNE::IModelGPU> Model = Runtime->CreateModelGPU(ModelData);
	if (!Model.IsValid())
	{
		UE_LOG(LogKinemotion, Error, TEXT("Runtime '%s' could not build a model from the pose asset."),
			*NneRuntimeName.ToString());
		return;
	}

	ModelInstance = Model->CreateModelInstanceGPU();
	if (!ModelInstance.IsValid())
	{
		UE_LOG(LogKinemotion, Error, TEXT("Failed to create a model instance."));
		return;
	}

	const TArray<UE::NNE::FTensorShape> InputShapes =
	{
		UE::NNE::FTensorShape::Make({
			1,
			static_cast<uint32>(KinemotionModel::InputChannels),
			static_cast<uint32>(KinemotionModel::InputHeight),
			static_cast<uint32>(KinemotionModel::InputWidth)
		})
	};

	if (ModelInstance->SetInputTensorShapes(InputShapes) != UE::NNE::EResultStatus::Ok)
	{
		UE_LOG(LogKinemotion, Error, TEXT("Model rejected the input shape %dx%dx%d."),
			KinemotionModel::InputChannels, KinemotionModel::InputHeight, KinemotionModel::InputWidth);
		ModelInstance.Reset();
		return;
	}

	UE_LOG(LogKinemotion, Log, TEXT("Pose model initialised on runtime '%s'."), *NneRuntimeName.ToString());
}

void UKinemotionMocap::InitMedia()
{
	MediaPlayer = NewObject<UMediaPlayer>(this);
	MediaTexture = NewObject<UMediaTexture>(this);
	MediaTexture->SetMediaPlayer(MediaPlayer);
	MediaTexture->UpdateResource();

	TArray<FMediaCaptureDevice> Devices;
	UMediaBlueprintFunctionLibrary::EnumerateVideoCaptureDevices(Devices, AllCaptureDeviceFilter);

	if (!Devices.IsValidIndex(CameraIndex))
	{
		// Previously a silent no-op, which presented as the plugin simply not working.
		UE_LOG(LogKinemotion, Error, TEXT("CameraIndex %d is out of range; %d capture device(s) found."),
			CameraIndex, Devices.Num());
		return;
	}

	MediaPlayer->OpenUrl(Devices[CameraIndex].Url);
	MediaPlayer->Play();

	UE_LOG(LogKinemotion, Log, TEXT("Opened capture device %d of %d."), CameraIndex, Devices.Num());
}

// Calibration

void UKinemotionMocap::Recalibrate()
{
	bIsCalibrated = false;
	bPelvisInitialized = false;
	CalibrationAttempts = 0;

	CalibrateFromSkeleton();
}

void UKinemotionMocap::CalibrateFromSkeleton()
{
	USkeletalMeshComponent* SkelMesh = FindSkeletalMesh();
	if (!SkelMesh || !SkelMesh->GetSkeletalMeshAsset())
	{
		UE_LOG(LogKinemotion, Error, TEXT("Calibration needs a skeletal mesh on the owning actor."));
		return;
	}

	const FReferenceSkeleton& RefSkeleton = SkelMesh->GetSkeletalMeshAsset()->GetRefSkeleton();
	const int32 PelvisIdx = RefSkeleton.FindBoneIndex(PelvisBoneName);
	const int32 NeckIdx = RefSkeleton.FindBoneIndex(NeckBoneName);

	if (PelvisIdx == INDEX_NONE || NeckIdx == INDEX_NONE)
	{
		UE_LOG(LogKinemotion, Error,
			TEXT("Skeleton is missing '%s' or '%s'; Kinemotion expects the UE5 mannequin naming."),
			*PelvisBoneName.ToString(), *NeckBoneName.ToString());
		return;
	}

	const TArray<FTransform>& ComponentSpaceTransforms = SkelMesh->GetComponentSpaceTransforms();
	if (ComponentSpaceTransforms.Num() == 0)
	{
		// The mesh has not evaluated its pose yet on the first frame of play. Retry next tick,
		// but bounded: an unbounded retry against a mesh that never populates never terminates.
		if (++CalibrationAttempts > MaxCalibrationAttempts)
		{
			UE_LOG(LogKinemotion, Error,
				TEXT("Skeletal mesh produced no component-space transforms after %d attempts; giving up."),
				MaxCalibrationAttempts);
			return;
		}

		if (UWorld* World = GetWorld())
		{
			TWeakObjectPtr<UKinemotionMocap> WeakThis(this);
			World->GetTimerManager().SetTimerForNextTick([WeakThis]()
			{
				if (UKinemotionMocap* Self = WeakThis.Get())
				{
					Self->CalibrateFromSkeleton();
				}
			});
		}
		return;
	}

	if (!ComponentSpaceTransforms.IsValidIndex(PelvisIdx))
	{
		UE_LOG(LogKinemotion, Error, TEXT("Pelvis index %d is outside the evaluated pose."), PelvisIdx);
		return;
	}

	SkeletonBounds = SkelMesh->CalcBounds(FTransform::Identity).GetBox();
	SkeletonHeight = SkeletonBounds.GetSize().Z;

	const FVector PelvisLocation = ComponentSpaceTransforms[PelvisIdx].GetLocation();
	SkeletonRootOffset = PelvisLocation;

	// Floor anchor: the base of the mesh bounds, kept under the pelvis horizontally.
	FloorRootLocal = FVector(PelvisLocation.X, PelvisLocation.Y, SkeletonBounds.Min.Z + FloorZOffset);

	CalculateEffectiveScale();

	// A pose measured against the previous calibration is not comparable to one measured
	// against this, so the filter history is dropped rather than carried over.
	PrevFilteredLocal.Reset();
	PrevFilteredLocal.SetNumZeroed(KinemotionPointCount);
	bHasFilteredSample.Init(false, KinemotionPointCount);

	bIsCalibrated = true;
	CalibrationAttempts = 0;

	UE_LOG(LogKinemotion, Log,
		TEXT("Calibrated. Height %.1f cm, pelvis Z %.1f, floor Z %.1f, scale %.1f."),
		SkeletonHeight, PelvisLocation.Z, FloorRootLocal.Z, EffectiveIsotropicScale);
}

void UKinemotionMocap::CalculateEffectiveScale()
{
	const float ReferenceLength = bScaleRelativeToSkeleton ? SkeletonHeight : ReferenceSizeCM;

	// A single factor for all three axes. Per-axis scaling was exposed previously but never
	// applied, so poses could not be stretched independently and the properties did nothing.
	EffectiveIsotropicScale = FMath::Max(1.0f, ReferenceLength) * FMath::Max(0.1f, StickmanScale);
}

// Frame acquisition

void UKinemotionMocap::RequestTextureRead()
{
	if (!MediaTexture)
	{
		return;
	}

	// Resolved here, on the game thread. The render-thread lambda must not touch the UObject:
	// it may be collected while the command is queued.
	FTextureResource* Resource = MediaTexture->GetResource();
	if (!Resource)
	{
		return;
	}

	bIsReadingFrame.store(true, std::memory_order_release);

	// A weak pointer rather than a raw `this`. The previous version captured the component
	// directly and wrote through it from both the render thread and a follow-up game-thread
	// task, either of which can outlive a component destroyed mid-flight.
	TWeakObjectPtr<UKinemotionMocap> WeakThis(this);

	ENQUEUE_RENDER_COMMAND(KinemotionReadWebcamFrame)(
		[WeakThis, Resource](FRHICommandListImmediate& RHICmdList)
		{
			const auto AbortRead = [WeakThis]()
			{
				AsyncTask(ENamedThreads::GameThread, [WeakThis]()
				{
					if (UKinemotionMocap* Self = WeakThis.Get())
					{
						Self->OnFrameReadComplete(TArray<FColor>());
					}
				});
			};

			FRHITexture* TextureRHI = Resource->GetTextureRHI();
			if (!TextureRHI || !TextureRHI->GetTexture2D())
			{
				AbortRead();
				return;
			}

			const FIntPoint Size = TextureRHI->GetSizeXY();

			TArray<FColor> Pixels;
			FReadSurfaceDataFlags ReadFlags(RCM_UNorm);
			ReadFlags.SetLinearToGamma(false);

			RHICmdList.ReadSurfaceData(TextureRHI, FIntRect(0, 0, Size.X, Size.Y), Pixels, ReadFlags);

			AsyncTask(ENamedThreads::GameThread, [WeakThis, Pixels = MoveTemp(Pixels)]() mutable
			{
				if (UKinemotionMocap* Self = WeakThis.Get())
				{
					Self->OnFrameReadComplete(MoveTemp(Pixels));
				}
			});
		});
}

void UKinemotionMocap::OnFrameReadComplete(TArray<FColor>&& Pixels)
{
	RawPixels = MoveTemp(Pixels);

	if (RawPixels.Num() > 0)
	{
		PreProcessImage();
		RunInference();
	}

	// Cleared last, so the next tick cannot queue a read while this one is still decoding.
	bIsReadingFrame.store(false, std::memory_order_release);
}

void UKinemotionMocap::PreProcessImage()
{
	if (!MediaTexture)
	{
		return;
	}

	const int32 SourceWidth = MediaTexture->GetWidth();
	const int32 SourceHeight = MediaTexture->GetHeight();
	if (SourceWidth <= 0 || SourceHeight <= 0)
	{
		return;
	}

	if (InputTensor.Num() != KinemotionModel::InputElementCount)
	{
		InputTensor.SetNumUninitialized(KinemotionModel::InputElementCount);
	}

	// Nearest-neighbour resample from the camera frame to the network's input resolution.
	const float SampleScaleX = static_cast<float>(SourceWidth) / static_cast<float>(KinemotionModel::InputWidth);
	const float SampleScaleY = static_cast<float>(SourceHeight) / static_cast<float>(KinemotionModel::InputHeight);

	// Planar layout: the model expects all red, then all green, then all blue.
	constexpr int32 PlaneStride = KinemotionModel::InputWidth * KinemotionModel::InputHeight;
	float* RedPlane = InputTensor.GetData();
	float* GreenPlane = RedPlane + PlaneStride;
	float* BluePlane = GreenPlane + PlaneStride;

	for (int32 Y = 0; Y < KinemotionModel::InputHeight; ++Y)
	{
		const int32 SourceY = FMath::FloorToInt32(Y * SampleScaleY);

		for (int32 X = 0; X < KinemotionModel::InputWidth; ++X)
		{
			const int32 SourceX = FMath::FloorToInt32(X * SampleScaleX);
			const int32 SourceIndex = (SourceY * SourceWidth) + SourceX;
			const int32 DestIndex = (Y * KinemotionModel::InputWidth) + X;

			if (!RawPixels.IsValidIndex(SourceIndex))
			{
				continue;
			}

			const FColor& Pixel = RawPixels[SourceIndex];
			RedPlane[DestIndex] = Pixel.R / KinemotionModel::ColorChannelMax;
			GreenPlane[DestIndex] = Pixel.G / KinemotionModel::ColorChannelMax;
			BluePlane[DestIndex] = Pixel.B / KinemotionModel::ColorChannelMax;
		}
	}
}

void UKinemotionMocap::RunInference()
{
	if (!ModelInstance.IsValid())
	{
		return;
	}

	if (OutputTensorX.Num() != KinemotionModel::OutputElementCountX)
	{
		OutputTensorX.SetNumUninitialized(KinemotionModel::OutputElementCountX);
	}
	if (OutputTensorY.Num() != KinemotionModel::OutputElementCountY)
	{
		OutputTensorY.SetNumUninitialized(KinemotionModel::OutputElementCountY);
	}
	if (OutputTensorZ.Num() != KinemotionModel::OutputElementCountZ)
	{
		OutputTensorZ.SetNumUninitialized(KinemotionModel::OutputElementCountZ);
	}

	const auto BindTensor = [](UE::NNE::FTensorBindingCPU& Binding, TArray<float>& Storage)
	{
		Binding.Data = Storage.GetData();
		Binding.SizeInBytes = Storage.Num() * sizeof(float);
	};

	InputBindings.SetNum(1);
	BindTensor(InputBindings[0], InputTensor);

	OutputBindings.SetNum(3);
	BindTensor(OutputBindings[0], OutputTensorX);
	BindTensor(OutputBindings[1], OutputTensorY);
	BindTensor(OutputBindings[2], OutputTensorZ);

	if (ModelInstance->RunSync(InputBindings, OutputBindings) == UE::NNE::EResultStatus::Ok)
	{
		DecodeAndPublish();
	}
}

// Decode and publish

void UKinemotionMocap::DecodeAndPublish()
{
	if (!bLiveLinkRegistered || !bIsCalibrated)
	{
		return;
	}

	USkeletalMeshComponent* SkelMesh = FindSkeletalMesh();
	if (!SkelMesh)
	{
		return;
	}

	if (!bPelvisInitialized && SkelMesh->GetBoneIndex(PelvisBoneName) != INDEX_NONE)
	{
		InitialPelvisWorld = SkelMesh->GetBoneLocation(PelvisBoneName);
		bPelvisInitialized = true;
	}

	// Decode

	const auto DecodeKeypoint = [this](int32 ModelIndex) -> FVector
	{
		const float* AxisX = OutputTensorX.GetData() + (ModelIndex * KinemotionModel::SimccBinsX);
		const float* AxisY = OutputTensorY.GetData() + (ModelIndex * KinemotionModel::SimccBinsY);
		const float* AxisZ = OutputTensorZ.GetData() + (ModelIndex * KinemotionModel::SimccBinsZ);

		// The model's axes do not match Unreal's: its Z is depth, its X is image width, and
		// its Y grows downward - hence the inversion on height.
		const FVector Decoded(
			DecodeSimccAxis(AxisZ, KinemotionModel::SimccBinsZ),
			DecodeSimccAxis(AxisX, KinemotionModel::SimccBinsX),
			1.0f - DecodeSimccAxis(AxisY, KinemotionModel::SimccBinsY));

		return Decoded.ContainsNaN() ? FVector::ZeroVector : Decoded;
	};

	TArray<FVector> RawPose;
	RawPose.SetNumZeroed(KinemotionPointCount);

	for (const FKinemotionKeypointMapping& Mapping : KinemotionDecodedPoints)
	{
		RawPose[static_cast<int32>(Mapping.Point)] = DecodeKeypoint(Mapping.ModelIndex);
	}

	// Derive the points the model does not predict

	const FVector LeftHip = RawPose[static_cast<int32>(EKinemotionPoint::LeftHip)];
	const FVector RightHip = RawPose[static_cast<int32>(EKinemotionPoint::RightHip)];
	const FVector LeftShoulder = RawPose[static_cast<int32>(EKinemotionPoint::LeftShoulder)];
	const FVector RightShoulder = RawPose[static_cast<int32>(EKinemotionPoint::RightShoulder)];

	const FVector HipCenter = (LeftHip + RightHip) * SymmetricPairMidpoint;
	const FVector NeckBase = (LeftShoulder + RightShoulder) * SymmetricPairMidpoint;

	RawPose[static_cast<int32>(EKinemotionPoint::HipCenter)] = HipCenter;
	RawPose[static_cast<int32>(EKinemotionPoint::NeckBase)] = NeckBase;
	RawPose[static_cast<int32>(EKinemotionPoint::SpineMid)] = (HipCenter + NeckBase) * SymmetricPairMidpoint;
	RawPose[static_cast<int32>(EKinemotionPoint::LeftClavicle)] = FMath::Lerp(NeckBase, LeftShoulder, ClavicleBlend);
	RawPose[static_cast<int32>(EKinemotionPoint::RightClavicle)] = FMath::Lerp(NeckBase, RightShoulder, ClavicleBlend);

	// Into component space

	// Centring, scaling, axis correction, the root offset and smoothing used to be five
	// sequential passes over eighteen individually named locals. One pass, one array.
	const FQuat AxisCorrection(FRotator(0.0f, YawCorrectionDegrees, 0.0f));

	for (int32 PointIndex = 0; PointIndex < KinemotionPointCount; ++PointIndex)
	{
		const FVector Centered = (RawPose[PointIndex] - HipCenter) * EffectiveIsotropicScale;
		const FVector Oriented = AxisCorrection.RotateVector(Centered) + SkeletonRootOffset;

		PoseLocal[PointIndex] = ApplySmoothing(static_cast<EKinemotionPoint>(PointIndex), Oriented);
	}

	// Into world space, for consumers

	const FTransform ComponentTransform = SkelMesh->GetComponentTransform();

	// Anchor the capture on the mesh's actual pelvis so the tracked figure and the mesh
	// occupy the same place rather than drifting apart.
	FVector WorldOffset = FVector::ZeroVector;
	if (bPelvisInitialized)
	{
		const FVector TrackedPelvisWorld =
			ComponentTransform.TransformPosition(PoseLocal[static_cast<int32>(EKinemotionPoint::HipCenter)]);
		WorldOffset = SkelMesh->GetBoneLocation(PelvisBoneName) - TrackedPelvisWorld;
	}

	CachedWorldPoints.Reset();
	CachedWorldPoints.Reserve(KinemotionPointCount);
	for (int32 PointIndex = 0; PointIndex < KinemotionPointCount; ++PointIndex)
	{
		CachedWorldPoints.Add(
			static_cast<EKinemotionPoint>(PointIndex),
			ComponentTransform.TransformPosition(PoseLocal[PointIndex]) + WorldOffset);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		if (const UWorld* World = GetWorld())
		{
			DrawDebugPose(*World);
		}
	}
#endif

	// Publish

	FLiveLinkFrameDataStruct FrameData(FLiveLinkAnimationFrameData::StaticStruct());
	FLiveLinkAnimationFrameData* AnimData = FrameData.Cast<FLiveLinkAnimationFrameData>();
	AnimData->Transforms.SetNum(KinemotionBoneCount);

	const FVector RootLocal = bUseFloorAsRoot ? FloorRootLocal : SkeletonRootOffset;
	AnimData->Transforms[RootBoneIndex] = FTransform(FQuat::Identity, RootLocal, FVector::OneVector);

	FVector PelvisLocal = PoseLocal[static_cast<int32>(EKinemotionPoint::HipCenter)] - RootLocal;
	if (!bPelvisFree)
	{
		// Pinned vertically: horizontal movement still tracks, but the pelvis keeps the height
		// it had at calibration so the figure cannot sink or float.
		PelvisLocal.Z = SkeletonRootOffset.Z - RootLocal.Z;
	}
	AnimData->Transforms[PelvisBoneIndex] = FTransform(FQuat::Identity, PelvisLocal, FVector::OneVector);

	for (int32 BoneIndex = FirstDrivenBoneIndex; BoneIndex < KinemotionBoneCount; ++BoneIndex)
	{
		const FKinemotionBoneDef& Bone = KinemotionRig[BoneIndex];
		const FVector BoneLocal =
			PoseLocal[static_cast<int32>(Bone.Point)] - PoseLocal[static_cast<int32>(Bone.ReferencePoint)];

		AnimData->Transforms[BoneIndex] = FTransform(FQuat::Identity, BoneLocal, FVector::OneVector);
	}

	if (IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		ILiveLinkClient& LiveLinkClient =
			IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);

		AnimData->MetaData.SceneTime = FQualifiedFrameTime(FTimecode(), FFrameRate(LiveLinkFrameRate, 1));

		LiveLinkClient.PushSubjectFrameData_AnyThread(SubjectKey, MoveTemp(FrameData));
	}
}

TMap<EKinemotionPoint, FVector> UKinemotionMocap::GetDetectedBodyPoints() const
{
	return CachedWorldPoints;
}

// Filtering

FVector UKinemotionMocap::ApplySmoothing(EKinemotionPoint PointId, const FVector& NewPos)
{
	const int32 PointIndex = static_cast<int32>(PointId);
	if (!PrevFilteredLocal.IsValidIndex(PointIndex))
	{
		return NewPos;
	}

	const auto Accept = [this, PointIndex](const FVector& Value) -> FVector
	{
		PrevFilteredLocal[PointIndex] = Value;
		bHasFilteredSample[PointIndex] = true;
		return Value;
	};

	if (!bEnableSmoothing || SmoothingAlpha <= MinEffectiveSmoothingAlpha)
	{
		return Accept(NewPos);
	}

	// Nothing to smooth against on the first sample for this point.
	if (!bHasFilteredSample[PointIndex])
	{
		return Accept(NewPos);
	}

	const FVector Previous = PrevFilteredLocal[PointIndex];
	FVector Delta = NewPos - Previous;
	const float DeltaSize = Delta.Size();

	// Deadzone: hold position through detection noise rather than shivering in place.
	if (DeltaSize < DeadzoneCM)
	{
		return Previous;
	}

	// Step clamp: a misdetection should drag the point, not teleport it.
	if (DeltaSize > MaxStepCM)
	{
		Delta = Delta.GetSafeNormal() * MaxStepCM;
	}

	return Accept(Previous + Delta * (1.0f - SmoothingAlpha));
}

// Helpers

USkeletalMeshComponent* UKinemotionMocap::FindSkeletalMesh() const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	USkeletalMeshComponent* SkelMesh = Owner->FindComponentByClass<USkeletalMeshComponent>();

	// Logged once rather than every frame: this runs inside the capture path.
	if (!SkelMesh && !bLoggedMissingSkeletalMesh)
	{
		bLoggedMissingSkeletalMesh = true;
		UE_LOG(LogKinemotion, Error,
			TEXT("%s has no USkeletalMeshComponent; Kinemotion has nothing to drive."), *Owner->GetName());
	}

	return SkelMesh;
}

#if ENABLE_DRAW_DEBUG
void UKinemotionMocap::DrawDebugPose(const UWorld& World) const
{
	for (const TPair<EKinemotionPoint, FVector>& Point : CachedWorldPoints)
	{
		DrawDebugSphere(&World, Point.Value, DebugPointRadius, DebugSphereSegments,
			FColor::Red, false, DebugDrawLifetime, SDPG_World, DebugBoneThickness);
	}

	// Bones come from the same rig table that drives the published skeleton, so the debug
	// view cannot disagree with what is actually being sent.
	for (int32 BoneIndex = FirstDrivenBoneIndex; BoneIndex < KinemotionBoneCount; ++BoneIndex)
	{
		const FKinemotionBoneDef& Bone = KinemotionRig[BoneIndex];
		const FVector* Start = CachedWorldPoints.Find(Bone.ReferencePoint);
		const FVector* End = CachedWorldPoints.Find(Bone.Point);

		if (Start && End)
		{
			DrawDebugLine(&World, *Start, *End, FColor::Green, false,
				DebugDrawLifetime, SDPG_World, DebugBoneThickness);
		}
	}
}
#endif
