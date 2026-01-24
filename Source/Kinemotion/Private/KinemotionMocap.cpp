#include "KinemotionMocap.h"
#include "Misc/MediaBlueprintFunctionLibrary.h"
#include "Rendering/Texture2DResource.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkAnimationTypes.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Async/Async.h"
#include "Misc/Timecode.h"
#include "NNEModelData.h" 
#include "Features/IModularFeatures.h" // Obligatorio para el Cliente Directo

// Función auxiliar para crear rotación desde dirección (Forward-X) y UpVector (Z)
static FQuat MakeQuatFromDirection(const FVector& Dir, const FVector& UpVector = FVector::UpVector)
{
	const FVector Forward = Dir.GetSafeNormal();
	const FVector Right = FVector::CrossProduct(UpVector, Forward).GetSafeNormal();
	const FVector RecomputedUp = FVector::CrossProduct(Forward, Right).GetSafeNormal();

	FMatrix Mat = FMatrix(
		Forward,
		Right,
		RecomputedUp,
		FVector::ZeroVector
	);

	return FQuat(Mat);
}

UKinemotionMocap::UKinemotionMocap()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UKinemotionMocap::BeginPlay()
{
	Super::BeginPlay();

	// 1. Configurar Live Link Directo (Prioritario)
	SetupLiveLinkDirect();

	// 2. Cargar IA
	InitNNE();

	// 3. **CALIBRAR SKELETON** ⭐
	if (bAutoCalibrate)
	{
		CalibrateFromSkeleton();
	}
	// 3. Abrir Cámara
	InitMedia();
}

void UKinemotionMocap::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Resetear estado de pelvis
	bPelvisInitialized = false;
	InitialPelvisWorld = FVector::ZeroVector;
	if (MediaPlayer) MediaPlayer->Close();

	// Limpiar sujeto de Live Link para no dejar "fantasmas"
	if (bLiveLinkRegistered && IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		ILiveLinkClient* LiveLinkClient = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
		LiveLinkClient->RemoveSubject_AnyThread(SubjectKey);
	}

	ModelInstance.Reset();
	Super::EndPlay(EndPlayReason);
}

void UKinemotionMocap::SetupLiveLinkDirect()
{
	// Obtener la interfaz del cliente del motor
	if (!IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		UE_LOG(LogTemp, Error, TEXT("[Kinemotion] LiveLink Plugin is NOT enabled. Please enable it."));
		return;
	}

	ILiveLinkClient* LiveLinkClient = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);

	// CREAR Y REGISTRAR LA SOURCE PRIMERO (Esto hace que aparezca en la UI)
	LiveLinkSource = MakeShared<FKinemotionLiveLinkSource>(
		FText::FromString(TEXT("Kinemotion Mocap"))
	);

	// AddSource asigna el GUID internamente y llama a ReceiveClient
	SourceGuid = LiveLinkClient->AddSource(LiveLinkSource);

	// Ahora crear el Subject Key con el GUID correcto de la Source
	SubjectKey = FLiveLinkSubjectKey(SourceGuid, FName(*SubjectName));

	// Definir Estructura Estática
	FLiveLinkStaticDataStruct StaticData(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData* SkelData = StaticData.Cast<FLiveLinkSkeletonStaticData>();

	// --- LISTA DE HUESOS AMPLIADA ---
	SkelData->BoneNames = {
		TEXT("root"),           // 0
		TEXT("pelvis"),         // 1
		TEXT("spine_01"),       // 2  <-- NUEVO
		TEXT("neck_01"),        // 3  <-- NUEVO
		TEXT("head"),           // 4  (Antes era 2)

		TEXT("clavicle_l"),     // 5  <-- NUEVO
		TEXT("upperarm_l"),     // 6
		TEXT("lowerarm_l"),     // 7
		TEXT("hand_l"),         // 8

		TEXT("clavicle_r"),     // 9  <-- NUEVO
		TEXT("upperarm_r"),     // 10
		TEXT("lowerarm_r"),     // 11
		TEXT("hand_r"),         // 12

		TEXT("thigh_l"),        // 13
		TEXT("calf_l"),         // 14
		TEXT("foot_l"),         // 15

		TEXT("thigh_r"),        // 16
		TEXT("calf_r"),         // 17
		TEXT("foot_r")          // 18
	};

	// --- JERARQUÍA ACTUALIZADA ---
	SkelData->BoneParents = {
		-1, // 0 root
		 0, // 1 pelvis
		 1, // 2 spine_01   (Hijo de Pelvis)
		 2, // 3 neck_01    (Hijo de Spine)
		 3, // 4 head       (Hijo de Neck)

		 3	, // 5 clavicle_l (Hijo de Neck/Spine superior)
		 5, // 6 upperarm_l (Hijo de Clavicle)
		 6, // 7 lowerarm_l
		 7, // 8 hand_l

		 3, // 9 clavicle_r (Hijo de Neck/Spine superior)
		 9, // 10 upperarm_r
		 10,// 11 lowerarm_r
		 11,// 12 hand_r

		 1, // 13 thigh_l   (Hijo de Pelvis)
		 13,// 14 calf_l
		 14,// 15 foot_l

		 1, // 16 thigh_r   (Hijo de Pelvis)
		 16,// 17 calf_r
		 17 // 18 foot_r
	};

	// Inyectar directamente al motor
	LiveLinkClient->PushSubjectStaticData_AnyThread(SubjectKey, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));

	bLiveLinkRegistered = true;
	UE_LOG(LogTemp, Warning, TEXT("[Kinemotion] ✓ Live Link Source & Subject Registered: %s"), *SubjectName);
}

// 2. Inicializar IA (NNE) con Soporte GPU/CPU
// --- KinemotionMocap.cpp ---

void UKinemotionMocap::InitNNE()
{
    // 1. HARDCODE DE LA RUTA
    // La ruta es: /NombrePlugin/RutaRelativa/NombreArchivo.NombreArchivo
    const TCHAR* ModelPath = TEXT("/Kinemotion/NeuralNetworks/pose_landmark_full.pose_landmark_full");

    // 2. CARGAR MANUALMENTE
    UE_LOG(LogTemp, Log, TEXT("[Kinemotion] Attempting to hard-load model from: %s"), ModelPath);
    
    // StaticLoadObject busca el asset en memoria o lo carga del disco
    UObject* LoadedObject = StaticLoadObject(UNNEModelData::StaticClass(), nullptr, ModelPath);

    ModelData = Cast<UNNEModelData>(LoadedObject);

    if (!ModelData)
    {
        UE_LOG(LogTemp, Error, TEXT("[Kinemotion] CRITICAL ERROR: Could not load model at %s. Check Plugin Content is enabled and path is correct."), ModelPath);
        return;
    }

    // 3. CONTINUAR CON LA LÓGICA (Usando HardcodedModel)
    
    // Importante: Si estás probando el fix de CPU para empaquetar, usa esto:
    // TWeakInterfacePtr<INNERuntimeCPU> SelectedRuntime = UE::NNE::GetRuntime<INNERuntimeCPU>(TEXT("NNERuntimeORTCpu"));
    
    // Si vas a intentar GPU otra vez (ahora que el cook no debería tocar la referencia):
    TWeakInterfacePtr<INNERuntimeGPU> SelectedRuntime = UE::NNE::GetRuntime<INNERuntimeGPU>(TEXT("NNERuntimeORTDml"));

    if (!SelectedRuntime.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("[Kinemotion] No NNE Runtime found."));
        return;
    }
	
	TSharedPtr<UE::NNE::IModelGPU> Model = SelectedRuntime->CreateModelGPU(ModelData);
    
    if (Model.IsValid())
    {
    	ModelInstance = Model->CreateModelInstanceGPU();
        if (ModelInstance.IsValid())
        {
            InputHeight = 384;
            InputWidth = 288;

            TArray<UE::NNE::FTensorShape> InputShapes = {
                UE::NNE::FTensorShape::Make({1, 3, (uint32)InputHeight, (uint32)InputWidth})
            };

        	if (ModelInstance->SetInputTensorShapes(InputShapes) != UE::NNE::EResultStatus::Ok)
        	{
        		UE_LOG(LogTemp, Error, TEXT("[Kinemotion] Failed to set Input Shapes."));
        	}
        	else
        	{
        		UE_LOG(LogTemp, Warning, TEXT("[Kinemotion] Model Loaded & Initialized Successfully!"));
        	}
        }
    }
}
// --- WEBCAM ---
void UKinemotionMocap::InitMedia()
{
	MediaPlayer = NewObject<UMediaPlayer>(this);
	MediaTexture = NewObject<UMediaTexture>(this);
	MediaTexture->SetMediaPlayer(MediaPlayer);
	MediaTexture->UpdateResource();

	TArray<FMediaCaptureDevice> Devices;
	UMediaBlueprintFunctionLibrary::EnumerateVideoCaptureDevices(Devices, -1);

	if (Devices.IsValidIndex(CameraIndex))
	{
		MediaPlayer->OpenUrl(Devices[CameraIndex].Url);
		MediaPlayer->Play();
	}
}

void UKinemotionMocap::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsReadingFrame && ModelInstance && MediaTexture)
	{
		RequestTextureRead();
	}
}

void UKinemotionMocap::RequestTextureRead()
{
	if (!MediaTexture || !MediaTexture->GetResource()) return;

	bIsReadingFrame = true;

	ENQUEUE_RENDER_COMMAND(ReadWebcamFrame)(
		[this](FRHICommandListImmediate& RHICmdList)
		{
			FTextureResource* Resource = MediaTexture->GetResource();
			if (!Resource || !Resource->GetTextureRHI()) { bIsReadingFrame = false; return; }

			FTextureRHIRef TextureRHI = Resource->GetTextureRHI()->GetTexture2D();
			if (!TextureRHI) { bIsReadingFrame = false; return; }

			// Corrección de API: GetSizeX en RHI es válido
			const int32 W = TextureRHI->GetSizeX();
			const int32 H = TextureRHI->GetSizeY();

			TArray<FColor> LocalPixels;
			FReadSurfaceDataFlags ReadFlags(RCM_UNorm);
			ReadFlags.SetLinearToGamma(false);

			RHICmdList.ReadSurfaceData(TextureRHI, FIntRect(0, 0, W, H), LocalPixels, ReadFlags);

			AsyncTask(ENamedThreads::GameThread, [this, LocalPixels = MoveTemp(LocalPixels)]()
				{
					this->RawPixels = LocalPixels;
					this->OnFrameReadComplete();
				});
		}
		);
}

void UKinemotionMocap::OnFrameReadComplete()
{
	if (RawPixels.Num() > 0)
	{
		PreProcessImage();
		RunInference();
	}
	bIsReadingFrame = false;
}

void UKinemotionMocap::PreProcessImage()
{
	int32 NumElements = InputWidth * InputHeight * 3;
	if (InputTensor.Num() != NumElements) InputTensor.SetNumUninitialized(NumElements);

	int32 SrcW = MediaTexture->GetWidth();
	int32 SrcH = MediaTexture->GetHeight();
	if (SrcW <= 0) return;

	// ✅ Renombrado para evitar conflicto
	const float SampleScaleX = (float)SrcW / (float)InputWidth;
	const float SampleScaleY = (float)SrcH / (float)InputHeight;

	float* RPtr = InputTensor.GetData();
	float* GPtr = RPtr + (InputWidth * InputHeight);
	float* BPtr = GPtr + (InputWidth * InputHeight);

	for (int32 y = 0; y < InputHeight; y++)
	{
		for (int32 x = 0; x < InputWidth; x++)
		{
			int32 Sx = FMath::FloorToInt(x * SampleScaleX);
			int32 Sy = FMath::FloorToInt(y * SampleScaleY);
			int32 SrcIdx = (Sy * SrcW) + Sx;

			if (RawPixels.IsValidIndex(SrcIdx))
			{
				FColor Pixel = RawPixels[SrcIdx];
				int32 DestIdx = (y * InputWidth) + x;

				RPtr[DestIdx] = Pixel.R / 255.0f;
				GPtr[DestIdx] = Pixel.G / 255.0f;
				BPtr[DestIdx] = Pixel.B / 255.0f;
			}
		}
	}
}

void UKinemotionMocap::RunInference()
{
	if (!ModelInstance) return;

	InputBindings.SetNum(1);
	InputBindings[0].Data = InputTensor.GetData();
	InputBindings[0].SizeInBytes = InputTensor.Num() * sizeof(float);

	// Salidas SimCC (X: 133*576, Y: 133*768, Z: 133*576)
	int32 SizeX = 133 * 576;
	int32 SizeY = 133 * 768;
	int32 SizeZ = 133 * 576;

	if (OutputTensorX.Num() != SizeX) OutputTensorX.SetNumUninitialized(SizeX);
	if (OutputTensorY.Num() != SizeY) OutputTensorY.SetNumUninitialized(SizeY);
	if (OutputTensorZ.Num() != SizeZ) OutputTensorZ.SetNumUninitialized(SizeZ);

	OutputBindings.SetNum(3);
	OutputBindings[0].Data = OutputTensorX.GetData(); OutputBindings[0].SizeInBytes = OutputTensorX.Num() * sizeof(float);
	OutputBindings[1].Data = OutputTensorY.GetData(); OutputBindings[1].SizeInBytes = OutputTensorY.Num() * sizeof(float);
	OutputBindings[2].Data = OutputTensorZ.GetData(); OutputBindings[2].SizeInBytes = OutputTensorZ.Num() * sizeof(float);

	if (ModelInstance->RunSync(InputBindings, OutputBindings) == UE::NNE::EResultStatus::Ok)
	{
		DecodeAndSend(OutputTensorX, OutputTensorY, OutputTensorZ);
	}
}

float UKinemotionMocap::GetArgMax(const float* Buffer, int32 Size)
{
	float MaxVal = -FLT_MAX;
	int32 MaxIdx = 0;
	for (int32 i = 0; i < Size; i++) {
		if (Buffer[i] > MaxVal) { MaxVal = Buffer[i]; MaxIdx = i; }
	}
	return (float)MaxIdx / (float)Size;
}

void UKinemotionMocap::DecodeAndSend(const TArray<float>& X, const TArray<float>& Y, const TArray<float>& Z)
{
	if (!bLiveLinkRegistered || !bIsCalibrated) return;
	if (X.Num() == 0) return;

	if (!bPelvisInitialized)
	{
		USkeletalMeshComponent* SkelMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		if (SkelMesh)
		{
			// Obtener posición de la pelvis del skeleton en world space
			int32 PelvisIdx = SkelMesh->GetBoneIndex(FName("pelvis"));
			if (PelvisIdx != INDEX_NONE)
			{
				InitialPelvisWorld = SkelMesh->GetBoneLocation(FName("pelvis"));
				bPelvisInitialized = true;

				UE_LOG(LogTemp, Warning, TEXT("[Kinemotion] ✓ Pelvis inicial capturada: %s"), *InitialPelvisWorld.ToString());
			}
		}
	}

	const int32 NumJoints = 133;
	const int32 DimX = 576, DimY = 768, DimZ = 576;

	// -------------------------------------------------------------------------
	// A. DECODIFICAR KEYPOINTS NORMALIZADOS (0-1)
	// -------------------------------------------------------------------------
	auto GetDecodedPoint = [&](int32 JointIdx) -> FVector {
		if (JointIdx >= NumJoints) return FVector::ZeroVector;

		const float* PtrX = X.GetData() + (JointIdx * DimX);
		const float* PtrY = Y.GetData() + (JointIdx * DimY);
		const float* PtrZ = Z.GetData() + (JointIdx * DimZ);

		float NormX = GetArgMax(PtrX, DimX);
		float NormY = GetArgMax(PtrY, DimY);
		float NormZ = GetArgMax(PtrZ, DimZ);

		if (FMath::IsNaN(NormX)) return FVector::ZeroVector;

		FVector P;
		P.X = NormZ;           // Profundidad
		P.Y = NormX;           // Ancho
		P.Z = 1.0f - NormY;    // Alto (invertido)
		return P;
		};

	// Decodificar puntos
	FVector Nose = GetDecodedPoint(0);
	FVector LShoulder = GetDecodedPoint(5);
	FVector RShoulder = GetDecodedPoint(6);
	FVector LElbow = GetDecodedPoint(7);
	FVector RElbow = GetDecodedPoint(8);
	FVector LWrist = GetDecodedPoint(9);
	FVector RWrist = GetDecodedPoint(10);
	FVector LHip = GetDecodedPoint(11);
	FVector RHip = GetDecodedPoint(12);
	FVector LKnee = GetDecodedPoint(13);
	FVector RKnee = GetDecodedPoint(14);
	FVector LAnkle = GetDecodedPoint(15);
	FVector RAnkle = GetDecodedPoint(16);

	// Puntos virtuales
	FVector HipCenter = (LHip + RHip) * 0.5f;
	FVector NeckBase = (LShoulder + RShoulder) * 0.5f;
	FVector SpineMid = (HipCenter + NeckBase) * 0.5f;
	FVector LClavicle = FMath::Lerp(NeckBase, LShoulder, 0.3f);
	FVector RClavicle = FMath::Lerp(NeckBase, RShoulder, 0.3f);

	// -------------------------------------------------------------------------
	// B. CENTRAR Y ESCALAR (Isotrópico 1:1:1)
	// -------------------------------------------------------------------------
	auto NormalizeAndProject = [&](const FVector& Point) -> FVector {
		const FVector Centered = Point - HipCenter;
		return Centered * EffectiveIsotropicScale;
		};

	FVector NoseLocal = NormalizeAndProject(Nose);
	FVector LShoulderLocal = NormalizeAndProject(LShoulder);
	FVector RShoulderLocal = NormalizeAndProject(RShoulder);
	FVector LElbowLocal = NormalizeAndProject(LElbow);
	FVector RElbowLocal = NormalizeAndProject(RElbow);
	FVector LWristLocal = NormalizeAndProject(LWrist);
	FVector RWristLocal = NormalizeAndProject(RWrist);
	FVector LHipLocal = NormalizeAndProject(LHip);
	FVector RHipLocal = NormalizeAndProject(RHip);
	FVector LKneeLocal = NormalizeAndProject(LKnee);
	FVector RKneeLocal = NormalizeAndProject(RKnee);
	FVector LAnkleLocal = NormalizeAndProject(LAnkle);
	FVector RAnkleLocal = NormalizeAndProject(RAnkle);

	FVector HipCenterLocal = FVector::ZeroVector; // Origen antes de ajuste
	FVector NeckBaseLocal = NormalizeAndProject(NeckBase);
	FVector SpineMidLocal = NormalizeAndProject(SpineMid);
	FVector LClavicleLocal = NormalizeAndProject(LClavicle);
	FVector RClavicleLocal = NormalizeAndProject(RClavicle);

	// -------------------------------------------------------------------------
	// C. ROTACIÓN DE CORRECCIÓN
	// -------------------------------------------------------------------------
	FQuat LocalRotation = FQuat(FRotator(0.0f, 90.0f, 0.0f));

	auto RotateLocal = [&](const FVector& V) -> FVector {
		return LocalRotation.RotateVector(V);
		};

	NoseLocal = RotateLocal(NoseLocal);
	LShoulderLocal = RotateLocal(LShoulderLocal);
	RShoulderLocal = RotateLocal(RShoulderLocal);
	LElbowLocal = RotateLocal(LElbowLocal);
	RElbowLocal = RotateLocal(RElbowLocal);
	LWristLocal = RotateLocal(LWristLocal);
	RWristLocal = RotateLocal(RWristLocal);
	LHipLocal = RotateLocal(LHipLocal);
	RHipLocal = RotateLocal(RHipLocal);
	LKneeLocal = RotateLocal(LKneeLocal);
	RKneeLocal = RotateLocal(RKneeLocal);
	LAnkleLocal = RotateLocal(LAnkleLocal);
	RAnkleLocal = RotateLocal(RAnkleLocal);
	NeckBaseLocal = RotateLocal(NeckBaseLocal);
	SpineMidLocal = RotateLocal(SpineMidLocal);
	LClavicleLocal = RotateLocal(LClavicleLocal);
	RClavicleLocal = RotateLocal(RClavicleLocal);

	// -------------------------------------------------------------------------
	// D. DETERMINAR ROOT Y AJUSTAR A COMPONENT SPACE
	// -------------------------------------------------------------------------
	FVector RootLocal;

	if (bUseFloorAsRoot)
	{
		// Root en el suelo (base del bounding box)
		// X/Y siguen a la cadera detectada, Z fijo en el suelo
		RootLocal = FVector(
			FloorRootLocal.X + HipCenterLocal.X, // X sigue movimiento horizontal
			FloorRootLocal.Y + HipCenterLocal.Y, // Y sigue movimiento horizontal  
			FloorRootLocal.Z                      // Z fijo en el suelo
		);
	}
	else
	{
		// Root tradicional en la pelvis
		RootLocal = SkeletonRootOffset;
	}

	// Ajustar todos los puntos al component space
	auto AdjustToSkeletonSpace = [&](const FVector& LocalPos) -> FVector {
		if (bUseFloorAsRoot)
		{
			// Relativo al floor root
			return LocalPos + SkeletonRootOffset;
		}
		else
		{
			return LocalPos + SkeletonRootOffset;
		}
		};

	NoseLocal = AdjustToSkeletonSpace(NoseLocal);
	LShoulderLocal = AdjustToSkeletonSpace(LShoulderLocal);
	RShoulderLocal = AdjustToSkeletonSpace(RShoulderLocal);
	LElbowLocal = AdjustToSkeletonSpace(LElbowLocal);
	RElbowLocal = AdjustToSkeletonSpace(RElbowLocal);
	LWristLocal = AdjustToSkeletonSpace(LWristLocal);
	RWristLocal = AdjustToSkeletonSpace(RWristLocal);
	LHipLocal = AdjustToSkeletonSpace(LHipLocal);
	RHipLocal = AdjustToSkeletonSpace(RHipLocal);
	LKneeLocal = AdjustToSkeletonSpace(LKneeLocal);
	RKneeLocal = AdjustToSkeletonSpace(RKneeLocal);
	LAnkleLocal = AdjustToSkeletonSpace(LAnkleLocal);
	RAnkleLocal = AdjustToSkeletonSpace(RAnkleLocal);
	HipCenterLocal = AdjustToSkeletonSpace(HipCenterLocal);
	NeckBaseLocal = AdjustToSkeletonSpace(NeckBaseLocal);
	SpineMidLocal = AdjustToSkeletonSpace(SpineMidLocal);
	LClavicleLocal = AdjustToSkeletonSpace(LClavicleLocal);
	RClavicleLocal = AdjustToSkeletonSpace(RClavicleLocal);

	// -------------------------------------------------------------------------
	// E. APLICAR SUAVIZADO ANTI-JITTER
	// -------------------------------------------------------------------------
	NoseLocal = ApplySmoothing(EKinemotionPoint::Nose, NoseLocal);
	LShoulderLocal = ApplySmoothing(EKinemotionPoint::LeftShoulder, LShoulderLocal);
	RShoulderLocal = ApplySmoothing(EKinemotionPoint::RightShoulder, RShoulderLocal);
	LElbowLocal = ApplySmoothing(EKinemotionPoint::LeftElbow, LElbowLocal);
	RElbowLocal = ApplySmoothing(EKinemotionPoint::RightElbow, RElbowLocal);
	LWristLocal = ApplySmoothing(EKinemotionPoint::LeftWrist, LWristLocal);
	RWristLocal = ApplySmoothing(EKinemotionPoint::RightWrist, RWristLocal);
	LHipLocal = ApplySmoothing(EKinemotionPoint::LeftHip, LHipLocal);
	RHipLocal = ApplySmoothing(EKinemotionPoint::RightHip, RHipLocal);
	LKneeLocal = ApplySmoothing(EKinemotionPoint::LeftKnee, LKneeLocal);
	RKneeLocal = ApplySmoothing(EKinemotionPoint::RightKnee, RKneeLocal);
	LAnkleLocal = ApplySmoothing(EKinemotionPoint::LeftAnkle, LAnkleLocal);
	RAnkleLocal = ApplySmoothing(EKinemotionPoint::RightAnkle, RAnkleLocal);
	HipCenterLocal = ApplySmoothing(EKinemotionPoint::HipCenter, HipCenterLocal);
	NeckBaseLocal = ApplySmoothing(EKinemotionPoint::NeckBase, NeckBaseLocal);
	SpineMidLocal = ApplySmoothing(EKinemotionPoint::SpineMid, SpineMidLocal);
	LClavicleLocal = ApplySmoothing(EKinemotionPoint::LeftClavicle, LClavicleLocal);
	RClavicleLocal = ApplySmoothing(EKinemotionPoint::RightClavicle, RClavicleLocal);

	// -------------------------------------------------------------------------
	// F. CONVERTIR A WORLD SPACE
	// -------------------------------------------------------------------------
	USkeletalMeshComponent* SkelMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	if (!SkelMesh) return;

	FTransform ComponentTransform = SkelMesh->GetComponentTransform();

	// Calcular offset para que el stickman parta de la pelvis real del mesh
	FVector StickmanPelvisComponent = HipCenterLocal;
	FVector StickmanPelvisWorld = ComponentTransform.TransformPosition(StickmanPelvisComponent);

	// Offset entre la pelvis real inicial y donde estaría el stickman
	FVector WorldOffset = FVector::ZeroVector;
	if (bPelvisInitialized)
	{
		// Calcular diferencia solo en el primer frame o mantener el offset
		FVector CurrentMeshPelvis = SkelMesh->GetBoneLocation(FName("pelvis"));
		WorldOffset = CurrentMeshPelvis - StickmanPelvisWorld;
	}

	auto ToWorldSpace = [&](const FVector& ComponentPos) -> FVector {
		FVector WorldPos = ComponentTransform.TransformPosition(ComponentPos);
		return WorldPos + WorldOffset;
		};

	CachedWorldPoints.Empty();
	CachedWorldPoints.Add(EKinemotionPoint::Nose, ToWorldSpace(NoseLocal));
	CachedWorldPoints.Add(EKinemotionPoint::LeftShoulder, ToWorldSpace(LShoulderLocal));
	CachedWorldPoints.Add(EKinemotionPoint::RightShoulder, ToWorldSpace(RShoulderLocal));
	CachedWorldPoints.Add(EKinemotionPoint::LeftElbow, ToWorldSpace(LElbowLocal));
	CachedWorldPoints.Add(EKinemotionPoint::RightElbow, ToWorldSpace(RElbowLocal));
	CachedWorldPoints.Add(EKinemotionPoint::LeftWrist, ToWorldSpace(LWristLocal));
	CachedWorldPoints.Add(EKinemotionPoint::RightWrist, ToWorldSpace(RWristLocal));
	CachedWorldPoints.Add(EKinemotionPoint::LeftHip, ToWorldSpace(LHipLocal));
	CachedWorldPoints.Add(EKinemotionPoint::RightHip, ToWorldSpace(RHipLocal));
	CachedWorldPoints.Add(EKinemotionPoint::LeftKnee, ToWorldSpace(LKneeLocal));
	CachedWorldPoints.Add(EKinemotionPoint::RightKnee, ToWorldSpace(RKneeLocal));
	CachedWorldPoints.Add(EKinemotionPoint::LeftAnkle, ToWorldSpace(LAnkleLocal));
	CachedWorldPoints.Add(EKinemotionPoint::RightAnkle, ToWorldSpace(RAnkleLocal));
	CachedWorldPoints.Add(EKinemotionPoint::HipCenter, ToWorldSpace(HipCenterLocal));
	CachedWorldPoints.Add(EKinemotionPoint::NeckBase, ToWorldSpace(NeckBaseLocal));
	CachedWorldPoints.Add(EKinemotionPoint::SpineMid, ToWorldSpace(SpineMidLocal));
	CachedWorldPoints.Add(EKinemotionPoint::LeftClavicle, ToWorldSpace(LClavicleLocal));
	CachedWorldPoints.Add(EKinemotionPoint::RightClavicle, ToWorldSpace(RClavicleLocal));

	// -------------------------------------------------------------------------
	// G. DEBUG VISUAL (sin cambios significativos)
	// -------------------------------------------------------------------------
	if (bShowDebug && GetWorld())
	{
		// Dibujar esferas de los puntos
		for (const auto& Elem : CachedWorldPoints)
		{
			DrawDebugSphere(GetWorld(), Elem.Value, 5.0f, 8, FColor::Red, false, 0.05f, 0, 1.0f);
		}

		// Dibujar líneas de huesos
		auto DrawBone = [&](EKinemotionPoint Start, EKinemotionPoint End) {
			if (CachedWorldPoints.Contains(Start) && CachedWorldPoints.Contains(End))
			{
				DrawDebugLine(GetWorld(), CachedWorldPoints[Start], CachedWorldPoints[End],
					FColor::Green, false, 0.05f, 0, 2.0f);
			}
			};

		DrawBone(EKinemotionPoint::NeckBase, EKinemotionPoint::HipCenter);
		DrawBone(EKinemotionPoint::LeftShoulder, EKinemotionPoint::RightShoulder);
		DrawBone(EKinemotionPoint::LeftHip, EKinemotionPoint::RightHip);
		DrawBone(EKinemotionPoint::LeftShoulder, EKinemotionPoint::LeftElbow);
		DrawBone(EKinemotionPoint::LeftElbow, EKinemotionPoint::LeftWrist);
		DrawBone(EKinemotionPoint::RightShoulder, EKinemotionPoint::RightElbow);
		DrawBone(EKinemotionPoint::RightElbow, EKinemotionPoint::RightWrist);
		DrawBone(EKinemotionPoint::LeftHip, EKinemotionPoint::LeftKnee);
		DrawBone(EKinemotionPoint::LeftKnee, EKinemotionPoint::LeftAnkle);
		DrawBone(EKinemotionPoint::RightHip, EKinemotionPoint::RightKnee);
		DrawBone(EKinemotionPoint::RightKnee, EKinemotionPoint::RightAnkle);

		// Debug adicional
		FVector CompLocation = ComponentTransform.GetLocation();
		DrawDebugSphere(GetWorld(), CompLocation, 10.0f, 12, FColor::Yellow, false, 0.05f, 0, 2.0f);

		FVector PelvisWorld = ToWorldSpace(SkeletonRootOffset);
		DrawDebugSphere(GetWorld(), PelvisWorld, 8.0f, 12, FColor::Cyan, false, 0.05f, 0, 2.0f);

		FBox WorldBounds = SkeletonBounds.TransformBy(ComponentTransform);
		DrawDebugBox(GetWorld(), WorldBounds.GetCenter(), WorldBounds.GetExtent(),
			ComponentTransform.GetRotation(), FColor::Magenta, false, 0.05f, 0, 2.0f);

		// Floor root
		if (bUseFloorAsRoot)
		{
			FVector FloorWorld = ToWorldSpace(FloorRootLocal);
			DrawDebugSphere(GetWorld(), FloorWorld, 10.0f, 12, FColor::Blue, false, 0.05f, 0, 2.0f);
		}
	}

	// -------------------------------------------------------------------------
	// H. PREPARAR DATOS PARA LIVE LINK
	// -------------------------------------------------------------------------
	FLiveLinkFrameDataStruct FrameData(FLiveLinkAnimationFrameData::StaticStruct());
	FLiveLinkAnimationFrameData* AnimData = FrameData.Cast<FLiveLinkAnimationFrameData>();
	AnimData->Transforms.SetNum(19);

	auto SetBoneData = [&](int32 Index, const FVector& Location, const FVector& ParentLocation) {
		AnimData->Transforms[Index].SetLocation(Location - ParentLocation);
		AnimData->Transforms[Index].SetRotation(FQuat::Identity);
		AnimData->Transforms[Index].SetScale3D(FVector::OneVector);
		};

	// Calcular posición del root (base del bounding box)
	RootLocal = bUseFloorAsRoot ? FloorRootLocal : SkeletonRootOffset;

	// ROOT (index 0)
	AnimData->Transforms[0].SetLocation(RootLocal);
	AnimData->Transforms[0].SetRotation(FQuat::Identity);
	AnimData->Transforms[0].SetScale3D(FVector::OneVector);

	// PELVIS (index 1) - relativa al root
	if (bPelvisFree)
	{
		// Pelvis libre: puede moverse respecto al root
		AnimData->Transforms[1].SetLocation(HipCenterLocal - RootLocal);
	}
	else
	{
		// Pelvis fija verticalmente (solo X/Y siguen al tracking)
		FVector FixedPelvis = HipCenterLocal - RootLocal;
		FixedPelvis.Z = SkeletonRootOffset.Z - RootLocal.Z; // Mantener Z original
		AnimData->Transforms[1].SetLocation(FixedPelvis);
	}
	AnimData->Transforms[1].SetRotation(FQuat::Identity);
	AnimData->Transforms[1].SetScale3D(FVector::OneVector);

	// Referencia para el resto de huesos
	FVector PelvisRef = HipCenterLocal; // Siempre usar la posición real de la pelvis

	SetBoneData(2, SpineMidLocal, PelvisRef);      // spine_01
	SetBoneData(3, NeckBaseLocal, SpineMidLocal);  // neck_01
	SetBoneData(4, NoseLocal, NeckBaseLocal);      // head

	SetBoneData(5, LClavicleLocal, NeckBaseLocal);
	SetBoneData(6, LShoulderLocal, LClavicleLocal);
	SetBoneData(7, LElbowLocal, LShoulderLocal);
	SetBoneData(8, LWristLocal, LElbowLocal);

	SetBoneData(9, RClavicleLocal, NeckBaseLocal);
	SetBoneData(10, RShoulderLocal, RClavicleLocal);
	SetBoneData(11, RElbowLocal, RShoulderLocal);
	SetBoneData(12, RWristLocal, RElbowLocal);

	SetBoneData(13, LHipLocal, PelvisRef);
	SetBoneData(14, LKneeLocal, LHipLocal);
	SetBoneData(15, LAnkleLocal, LKneeLocal);

	SetBoneData(16, RHipLocal, PelvisRef);
	SetBoneData(17, RKneeLocal, RHipLocal);
	SetBoneData(18, RAnkleLocal, RKneeLocal);

	// -------------------------------------------------------------------------
	// I. ENVIAR A LIVE LINK
	// -------------------------------------------------------------------------
	if (IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		ILiveLinkClient* LiveLinkClient = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);

		FTimecode TC = FTimecode();
		AnimData->MetaData.SceneTime = FQualifiedFrameTime(TC, FFrameRate(60, 1));

		LiveLinkClient->PushSubjectFrameData_AnyThread(SubjectKey, MoveTemp(FrameData));
	}
}

TMap<EKinemotionPoint, FVector> UKinemotionMocap::GetDetectedBodyPoints()
{
	// Devuelve el mapa almacenado en el último frame procesado
	return CachedWorldPoints;
}

void UKinemotionMocap::CalibrateFromSkeleton()
{
	USkeletalMeshComponent* SkelMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	if (!SkelMesh || !SkelMesh->GetSkeletalMeshAsset())
	{
		UE_LOG(LogTemp, Error, TEXT("[Kinemotion] No SkeletalMeshComponent found!"));
		return;
	}

	const FReferenceSkeleton& RefSkeleton = SkelMesh->GetSkeletalMeshAsset()->GetRefSkeleton();
	int32 PelvisIdx = RefSkeleton.FindBoneIndex(FName("pelvis"));
	int32 NeckIdx = RefSkeleton.FindBoneIndex(FName("neck_01"));

	if (PelvisIdx == INDEX_NONE || NeckIdx == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("[Kinemotion] Required bones not found!"));
		return;
	}

	const TArray<FTransform>& CS = SkelMesh->GetComponentSpaceTransforms();
	if (CS.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Kinemotion] ComponentSpaceTransforms empty! Retrying..."));
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]() { CalibrateFromSkeleton(); });
		return;
	}

	if (!CS.IsValidIndex(PelvisIdx) || !CS.IsValidIndex(NeckIdx))
	{
		UE_LOG(LogTemp, Error, TEXT("[Kinemotion] Bone indices invalid!"));
		return;
	}

	// Bounding box del skeleton
	SkeletonBounds = SkelMesh->CalcBounds(FTransform::Identity).GetBox();
	const FVector BoundsSize = SkeletonBounds.GetSize();
	SkeletonHeight = BoundsSize.Z;

	// Pelvis en component space
	FVector PelvisLoc = CS[PelvisIdx].GetLocation();
	SkeletonRootOffset = PelvisLoc;

	// ✅ NUEVO: Root del suelo = base del bounding box (mismo X/Y que pelvis)
	const float FloorZ = SkeletonBounds.Min.Z + FloorZOffset;
	FloorRootLocal = FVector(PelvisLoc.X, PelvisLoc.Y, FloorZ);

	// Calcular escala
	CalculateEffectiveScale();

	// Limpiar buffer de suavizado
	PrevFilteredLocal.Empty();

	InitialSkeletonTransform = SkelMesh->GetComponentTransform();
	bIsCalibrated = true;

	UE_LOG(LogTemp, Warning, TEXT("[Kinemotion] ✓ Calibrated | Height: %.1f | PelvisZ: %.1f | FloorZ: %.1f | Scale: %.1f"),
		SkeletonHeight, PelvisLoc.Z, FloorRootLocal.Z, EffectiveIsotropicScale);
}

FVector UKinemotionMocap::ApplySmoothing(EKinemotionPoint PointId, const FVector& NewPos)
{
	if (!bEnableSmoothing || SmoothingAlpha <= 0.01f)
	{
		PrevFilteredLocal.Add(PointId, NewPos);
		return NewPos;
	}

	// Obtener posición anterior
	FVector* PrevPtr = PrevFilteredLocal.Find(PointId);
	if (!PrevPtr)
	{
		// Primera vez: guardar y retornar sin filtrar
		PrevFilteredLocal.Add(PointId, NewPos);
		return NewPos;
	}

	FVector Prev = *PrevPtr;
	FVector Delta = NewPos - Prev;
	float DeltaMag = Delta.Size();

	// 1. Deadzone: ignorar micro-movimientos
	if (DeltaMag < DeadzoneCM)
	{
		return Prev; // No actualizar
	}

	// 2. Clamp: limitar saltos bruscos
	if (DeltaMag > MaxStepCM)
	{
		Delta = Delta.GetSafeNormal() * MaxStepCM;
	}

	// 3. EMA (Exponential Moving Average)
	FVector Filtered = Prev + Delta * (1.0f - SmoothingAlpha);

	// Guardar para siguiente frame
	PrevFilteredLocal.Add(PointId, Filtered);
	return Filtered;
}

void UKinemotionMocap::CalculateEffectiveScale()
{
	// Base = altura del skeleton o tamaño fijo, según el modo elegido
	const float BaseRef = bScaleRelativeToSkeleton ? SkeletonHeight : ReferenceSizeCM;

	// Escala isotrópica: un único valor para X, Y, Z
	EffectiveIsotropicScale = FMath::Max(1.0f, BaseRef) * FMath::Max(0.1f, StickmanScale);
}

FVector UKinemotionMocap::ProjectToSkeletonSpace(const FVector& NormalizedPoint)
{
	// NormalizedPoint está en rango [0, 1] en X, Y, Z
	// Lo proyectamos al bounding box del skeleton

	FVector ProjectedLocal;

	// X (Profundidad): Mapear de [0,1] a [-Depth/2, +Depth/2]
	ProjectedLocal.X = (NormalizedPoint.X - 0.5f) * SkeletonDepth;

	// Y (Ancho): Mapear de [0,1] a [-Width/2, +Width/2]
	ProjectedLocal.Y = (NormalizedPoint.Y - 0.5f) * SkeletonWidth;

	// Z (Altura): Mapear de [0,1] a [0, Height] (desde pies hacia cabeza)
	ProjectedLocal.Z = NormalizedPoint.Z * SkeletonHeight;

	return ProjectedLocal;
}
