// Copyright (c) Victor Rivas Perez. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
// Defines ENABLE_DRAW_DEBUG. Without it the guarded declaration below would compile out here
// while the definition in the .cpp - which pulls the define in transitively - would not.
#include "EngineDefines.h"
#include "KinemotionLiveLinkSource.h"
#include "LiveLinkTypes.h"
#include "NNERuntimeCPU.h"
#include "NNERuntimeGPU.h"

#include <atomic>

#include "KinemotionMocap.generated.h"

class UMediaPlayer;
class UMediaTexture;
class UNNEModelData;
class USkeletalMeshComponent;

/**
 * Body points Kinemotion tracks and publishes.
 *
 * The first thirteen are decoded directly from the model. The remainder are derived from
 * those - the model has no keypoint for a hip centre or a clavicle, but the rig needs them.
 */
UENUM(BlueprintType)
enum class EKinemotionPoint : uint8
{
	Nose,
	LeftShoulder,
	RightShoulder,
	LeftElbow,
	RightElbow,
	LeftWrist,
	RightWrist,
	LeftHip,
	RightHip,
	LeftKnee,
	RightKnee,
	LeftAnkle,
	RightAnkle,

	// Derived points - computed from the decoded ones, not predicted by the model.
	HipCenter,
	NeckBase,
	SpineMid,
	LeftClavicle,
	RightClavicle,

	Count UMETA(Hidden)
};

/** Number of tracked points, derived from the enum so the two cannot drift apart. */
constexpr int32 KinemotionPointCount = static_cast<int32>(EKinemotionPoint::Count);

/**
 * Markerless motion capture from a webcam.
 *
 * Each frame the component reads the camera texture, runs a SimCC pose model over it through
 * NNE, converts the decoded keypoints into the owning skeletal mesh's space, and pushes the
 * result to Live Link as an animation subject. Add it to an actor that also has a
 * USkeletalMeshComponent; the mesh is what the capture is calibrated and scaled against.
 *
 * Frame acquisition is asynchronous: the texture read is issued on the render thread and its
 * result marshalled back to the game thread, with at most one read in flight at a time.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KINEMOTION_API UKinemotionMocap : public UActorComponent
{
	GENERATED_BODY()

public:
	UKinemotionMocap();

	//~ Begin UActorComponent interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent interface

	/** World-space positions of every tracked point, as of the last decoded frame. */
	UFUNCTION(BlueprintPure, Category = "Kinemotion")
	TMap<EKinemotionPoint, FVector> GetDetectedBodyPoints() const;

	/** True once the component has measured the owning skeletal mesh and can map poses onto it. */
	UFUNCTION(BlueprintPure, Category = "Kinemotion")
	bool IsCalibrated() const { return bIsCalibrated; }

	/**
	 * Re-measures the owning skeletal mesh and resets the smoothing history. Call after
	 * swapping the mesh, or after changing any of the scale properties at runtime.
	 */
	UFUNCTION(BlueprintCallable, Category = "Kinemotion")
	void Recalibrate();

protected:
	// -- Capture -----------------------------------------------------------------------

	/** Live Link subject name this component publishes under. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion")
	FString SubjectName;

	/** Index into the enumerated video capture devices. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion", meta = (ClampMin = "0"))
	int32 CameraIndex;

	// -- Model -------------------------------------------------------------------------

	/**
	 * Pose model driving the capture. Held as a soft reference so the asset is a real,
	 * cook-visible dependency of this component rather than a path resolved at runtime.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Model")
	TSoftObjectPtr<UNNEModelData> PoseModel;

	/** NNE runtime used to execute the model. Must be a registered GPU runtime. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Model")
	FName NneRuntimeName;

	// -- Scale -------------------------------------------------------------------------

	/** Multiplier applied on top of the measured reference size. 1.0 matches the skeleton. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Scale",
		meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float StickmanScale;

	/** Scale against the measured skeleton height, rather than against ReferenceSizeCM. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Scale")
	bool bScaleRelativeToSkeleton;

	/** Absolute reference height used when bScaleRelativeToSkeleton is false. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Scale",
		meta = (ClampMin = "10.0", EditCondition = "!bScaleRelativeToSkeleton", Units = "cm"))
	float ReferenceSizeCM;

	// -- Calibration -------------------------------------------------------------------

	/** Calibrate against the owning skeletal mesh automatically on BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Calibration")
	bool bAutoCalibrate;

	// -- Root --------------------------------------------------------------------------

	/** Anchor the rig root at the base of the mesh bounds rather than at the pelvis. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Root")
	bool bUseFloorAsRoot;

	/** Let the pelvis rise and fall with the tracked pose. When false its height is pinned. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Root")
	bool bPelvisFree;

	/** Vertical adjustment of the floor root, for meshes whose bounds do not sit on their feet. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Root",
		meta = (ClampMin = "-50.0", ClampMax = "50.0", Units = "cm"))
	float FloorZOffset;

	// -- Rig ---------------------------------------------------------------------------

	/**
	 * How far along the neck-to-shoulder line the derived clavicle points sit. 0 places them
	 * at the neck base, 1 at the shoulder.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Rig",
		meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ClavicleBlend;

	/**
	 * Yaw applied to the decoded pose to bring the model's axis convention onto Unreal's.
	 * Change only if the capture faces the wrong way.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Rig",
		meta = (ClampMin = "-180.0", ClampMax = "180.0", Units = "deg"))
	float YawCorrectionDegrees;

	/** Frame rate stamped on the published Live Link frames. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Rig",
		meta = (ClampMin = "1", ClampMax = "240"))
	int32 LiveLinkFrameRate;

	// -- Filter ------------------------------------------------------------------------

	/** Enables the anti-jitter filter applied to every tracked point. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Filter")
	bool bEnableSmoothing;

	/** Smoothing strength. 0 passes the raw pose through; higher is smoother but lags more. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Filter",
		meta = (ClampMin = "0.0", ClampMax = "0.95", EditCondition = "bEnableSmoothing"))
	float SmoothingAlpha;

	/** Movements below this are treated as noise and discarded. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Filter",
		meta = (ClampMin = "0.0", ClampMax = "5.0", EditCondition = "bEnableSmoothing", Units = "cm"))
	float DeadzoneCM;

	/** Upper bound on how far a point may travel in one frame, clamping detection spikes. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Filter",
		meta = (ClampMin = "1.0", ClampMax = "50.0", EditCondition = "bEnableSmoothing", Units = "cm"))
	float MaxStepCM;

	// -- Debug -------------------------------------------------------------------------

	/** Draws the tracked skeleton in the world. Compiled out in Shipping. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Debug")
	bool bShowDebug;

	/** Radius of the sphere drawn at each tracked point. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Debug",
		meta = (ClampMin = "0.1", EditCondition = "bShowDebug", Units = "cm"))
	float DebugPointRadius;

	/** Thickness of the lines drawn between tracked points. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Debug",
		meta = (ClampMin = "0.1", EditCondition = "bShowDebug"))
	float DebugBoneThickness;

	/**
	 * Lifetime of debug primitives. Kept just above a frame so the drawing refreshes with the
	 * capture instead of accumulating.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinemotion|Debug",
		meta = (ClampMin = "0.0", EditCondition = "bShowDebug", Units = "s"))
	float DebugDrawLifetime;

private:
	// -- Setup -------------------------------------------------------------------------

	void InitMedia();
	void InitNNE();
	void SetupLiveLinkDirect();
	void ShutdownLiveLink();

	/** Measures the owning skeletal mesh: bounds, pelvis offset, floor height, working scale. */
	void CalibrateFromSkeleton();

	/** Derives EffectiveIsotropicScale from the calibration and the scale properties. */
	void CalculateEffectiveScale();

	// -- Per-frame pipeline ------------------------------------------------------------

	/** Issues an asynchronous read of the media texture on the render thread. */
	void RequestTextureRead();

	/** Game-thread continuation of RequestTextureRead, handed the pixels that were read. */
	void OnFrameReadComplete(TArray<FColor>&& Pixels);

	/** Resamples RawPixels into the planar RGB input tensor. */
	void PreProcessImage();

	/** Runs the model and, on success, decodes and publishes the result. */
	void RunInference();

	/** Decodes the SimCC output into world-space points and pushes a Live Link frame. */
	void DecodeAndPublish();

	/** Applies the deadzone, step clamp and exponential smoothing for one point. */
	FVector ApplySmoothing(EKinemotionPoint PointId, const FVector& NewPos);

	/** Returns the owner's skeletal mesh component, or null with a warning logged once. */
	USkeletalMeshComponent* FindSkeletalMesh() const;

#if ENABLE_DRAW_DEBUG
	void DrawDebugPose(const UWorld& World) const;
#endif

	// -- Media -------------------------------------------------------------------------

	UPROPERTY(Transient)
	TObjectPtr<UMediaPlayer> MediaPlayer;

	UPROPERTY(Transient)
	TObjectPtr<UMediaTexture> MediaTexture;

	UPROPERTY(Transient)
	TObjectPtr<UNNEModelData> ModelData;

	TArray<FColor> RawPixels;

	/**
	 * Guards against queuing a second texture read while one is in flight. Written from the
	 * render thread on early-out and from the game thread on completion, hence atomic.
	 */
	std::atomic<bool> bIsReadingFrame{false};

	// -- Inference ---------------------------------------------------------------------

	TSharedPtr<UE::NNE::IModelInstanceGPU> ModelInstance;

	TArray<float> InputTensor;
	TArray<float> OutputTensorX;
	TArray<float> OutputTensorY;
	TArray<float> OutputTensorZ;

	TArray<UE::NNE::FTensorBindingCPU> InputBindings;
	TArray<UE::NNE::FTensorBindingCPU> OutputBindings;

	// -- Live Link ---------------------------------------------------------------------

	TSharedPtr<FKinemotionLiveLinkSource> LiveLinkSource;
	FGuid SourceGuid;
	FLiveLinkSubjectKey SubjectKey;
	bool bLiveLinkRegistered = false;

	// -- Calibration state -------------------------------------------------------------

	bool bIsCalibrated = false;

	/** Bounds of the owning mesh in component space, measured at calibration. */
	FBox SkeletonBounds;

	/** Total height of those bounds; the reference length when scaling relative to the skeleton. */
	float SkeletonHeight = 0.0f;

	/** Pelvis position in component space, used as the rig's non-floor root. */
	FVector SkeletonRootOffset = FVector::ZeroVector;

	/** Floor anchor in component space: pelvis X/Y at the base of the bounds. */
	FVector FloorRootLocal = FVector::ZeroVector;

	/** Single factor converting the model's normalised output into component-space centimetres. */
	float EffectiveIsotropicScale = 0.0f;

	/** Next-tick calibration retries used so far, bounding the wait for the mesh to evaluate. */
	int32 CalibrationAttempts = 0;

	// -- Per-frame state ---------------------------------------------------------------

	/** Latest decoded pose in component space, indexed by EKinemotionPoint. */
	TArray<FVector> PoseLocal;

	/** Latest decoded pose in world space, published through GetDetectedBodyPoints(). */
	TMap<EKinemotionPoint, FVector> CachedWorldPoints;

	/** Previous filtered position per point, backing the anti-jitter filter. */
	TArray<FVector> PrevFilteredLocal;

	/** Whether PrevFilteredLocal holds a usable sample for the matching point. */
	TArray<bool> bHasFilteredSample;

	/** World-space pelvis of the mesh when tracking began, used to anchor the capture. */
	FVector InitialPelvisWorld = FVector::ZeroVector;
	bool bPelvisInitialized = false;

	/** Set once the missing-skeletal-mesh warning has been logged, to keep it off the tick. */
	mutable bool bLoggedMissingSkeletalMesh = false;
};
