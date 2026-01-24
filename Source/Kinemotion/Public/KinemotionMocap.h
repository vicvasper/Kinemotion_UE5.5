#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "NNE.h"
#include "Components/SkeletalMeshComponent.h" // Arregla C2027: USkeletalMeshComponent
#include "Engine/SkeletalMesh.h"              // Arregla acceso a GetRefSkeleton()
#include "DrawDebugHelpers.h"                 // Arregla C3861: DrawDebugSphere, DrawDebugLine
#include "Engine/World.h"                     // Arregla C2027: UWorld, GetWorld()
#include "TimerManager.h"                     // Arregla GetTimerManager()
#include "NNERuntime.h"
#include "NNERuntimeGPU.h"
#include "NNERuntimeCPU.h"
#include "ILiveLinkClient.h"
#include "ILiveLinkSource.h"  // A�ADIR ESTE
#include "Roles/LiveLinkAnimationTypes.h"
#include "KinemotionMocap.generated.h"

// Definici�n de huesos
namespace KineBoneNames
{
	// Usamos nombres est�ndar de UE5 Mannequin para facilitar el rigging
	static const FName Root = "root";
	static const FName Hips = "pelvis";
	static const FName Head = "head";

	static const FName LeftArm = "upperarm_l";
	static const FName LeftForeArm = "lowerarm_l";
	static const FName LeftHand = "hand_l";

	static const FName RightArm = "upperarm_r";
	static const FName RightForeArm = "lowerarm_r";
	static const FName RightHand = "hand_r";

	static const FName LeftUpLeg = "thigh_l";
	static const FName LeftLeg = "calf_l";
	static const FName LeftFoot = "foot_l";

	static const FName RightUpLeg = "thigh_r";
	static const FName RightLeg = "calf_r";
	static const FName RightFoot = "foot_r";
}

UENUM(BlueprintType)
enum class EKinemotionPoint : uint8
{
	Nose,
	LeftShoulder, RightShoulder,
	LeftElbow, RightElbow,
	LeftWrist, RightWrist,
	LeftHip, RightHip,
	LeftKnee, RightKnee,
	LeftAnkle, RightAnkle,

	// --- PUNTOS VIRTUALES (Calculados) ---
	HipCenter,
	NeckBase,
	SpineMid,
	LeftClavicle,
	RightClavicle
};

// ============================================
// CLASE LIVELINK SOURCE (NECESARIA PARA QUE APAREZCA EN LA UI)
// ============================================
class FKinemotionLiveLinkSource : public ILiveLinkSource
{
public:
	FKinemotionLiveLinkSource(FText InSourceType) : SourceType(InSourceType) {}

	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override
	{
		Client = InClient;
		SourceGuid = InSourceGuid;
	}

	virtual bool IsSourceStillValid() const override { return true; }
	virtual bool RequestSourceShutdown() override { return true; }

	virtual FText GetSourceType() const override { return SourceType; }
	virtual FText GetSourceMachineName() const override { return FText::FromString(FPlatformProcess::ComputerName()); }
	virtual FText GetSourceStatus() const override { return NSLOCTEXT("Kinemotion", "Active", "Active"); }

private:
	FText SourceType;
	ILiveLinkClient* Client = nullptr;
	FGuid SourceGuid;
};

// ============================================
// COMPONENTE PRINCIPAL
// ============================================
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KINEMOTION_API UKinemotionMocap : public UActorComponent
{
	GENERATED_BODY()

public:
	UKinemotionMocap();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --- CONFIGURACI�N ---
	UPROPERTY()
	TObjectPtr<UNNEModelData> ModelData;

	UPROPERTY(EditAnywhere, Category = "Kinemotion")
	FString SubjectName = "KinemotionWebcam";

	UPROPERTY(EditAnywhere, Category = "Kinemotion")
	int32 CameraIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Kinemotion")
	float WorldScale = 170.0f;

	// SISTEMA DE ESCALADO AJUSTABLE
   // -------------------------------------------------------------------------

   /** Factor de escala global del stickman (1.0 = tama�o del skeleton, 2.0 = doble) */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Scale", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float GlobalScaleFactor = 1.5f;

	/** Escala en X (Profundidad/Adelante-Atr�s) */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Scale", meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float ScaleX = 1.0f;

	/** Escala en Y (Ancho/Izquierda-Derecha) - Aumenta para mejor movimiento lateral */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Scale", meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float ScaleY = 1.0f;

	/** Escala en Z (Altura/Arriba-Abajo) */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Scale", meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float ScaleZ = 1.0f;

	/** Auto-corregir proporciones aplastadas (recomendado) */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Scale")
	bool bAutoFixProportions = true;

	/** Mostrar informaci�n de calibraci�n en logs */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Debug")
	bool bDebugCalibration = true;

	UPROPERTY()
	bool bIsCalibrated = false;

	UPROPERTY(EditAnywhere, Category = "Kinemotion|Calibration")
	bool bAutoCalibrate = true;

	UPROPERTY(EditAnywhere, Category = "Kinemotion|Root")
	bool bUseFloorAsRoot = true;

	/** Si true, la pelvis puede subir/bajar libremente (para agacharse).
		Si false, la pelvis est� fija respecto al root. */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Root")
	bool bPelvisFree = false;

	/** Offset vertical del suelo (por si el mesh tiene suelas, etc.). */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Root", meta = (ClampMin = "-50.0", ClampMax = "50.0"))
	float FloorZOffset = 0.0f;

	// KinemotionMocap.h (en la secci�n public:)

/** Multiplicador de escala del stickman (1.0 = tama�o del skeleton, 2.0 = doble) */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Scale", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float StickmanScale = 1.0f;

	/** Usar altura del skeleton como referencia (true) o tama�o absoluto en cm (false) */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Scale")
	bool bScaleRelativeToSkeleton = true;

	/** Si bScaleRelativeToSkeleton = false, usar este tama�o base en cm */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Scale", meta = (ClampMin = "10.0", EditCondition = "!bScaleRelativeToSkeleton"))
	float ReferenceSizeCM = 170.0f;

	// Factores de escala calculados desde el skeleton real
	float SkeletonHeight = 0.0f;         // Altura total del modelo
	float TorsoLength = 0.0f;            // Pelvis -> Cuello
	float ArmLength = 0.0f;  
	float SkeletonWidth = 0.0f;   // Ancho de hombros
	float SkeletonDepth = 0.0f;   // Profundidad del bounding box// Hombro -> Mu�eca
	float LegLength = 0.0f;              // Cadera -> Tobillo
	float EffectiveIsotropicScale = 100.0f;

	// Offset vertical para alinear pelvis
	FVector SkeletonRootOffset = FVector::ZeroVector;

	FBox SkeletonBounds;          // Bounding box del skeleton
	FTransform InitialSkeletonTransform;

	// Factor de escala fijo calculado
	float FixedWorldScale = 1.0f;

	// Escalas efectivas calculadas
	FVector EffectiveScale = FVector::OneVector;

	// Proyectar punto normalizado (0-1) al espacio del skeleton
	FVector ProjectToSkeletonSpace(const FVector& NormalizedPoint);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Kinemotion AI")
	TMap<EKinemotionPoint, FVector> GetDetectedBodyPoints();

	// ============================================================================
// NUEVO: ANTI-JITTER
// ============================================================================
	/** Fuerza de suavizado [0..1]. 0 = sin suavizado, 0.3 = recomendado, 0.5+ = muy suave pero con lag. */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Filter", meta = (ClampMin = "0.0", ClampMax = "0.95"))
	float SmoothingAlpha = 0.3f;

	/** Zona muerta (cm). Movimientos menores a este valor se ignoran para reducir micro-jitter. */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Filter", meta = (ClampMin = "0.0", ClampMax = "5.0"))
	float DeadzoneCM = 0.5f;

	/** M�ximo desplazamiento por frame (cm). Limita picos/saltos. */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Filter", meta = (ClampMin = "1.0", ClampMax = "50.0"))
	float MaxStepCM = 15.0f;

	/** Activar/desactivar filtro. */
	UPROPERTY(EditAnywhere, Category = "Kinemotion|Filter")
	bool bEnableSmoothing = true;


	UPROPERTY(EditAnywhere, Category = "Kinemotion|Debug")
	bool bShowDebug = true;

	// Guardar offset inicial de pelvis en el mundo (private section)
	FVector InitialPelvisWorld = FVector::ZeroVector;
	bool bPelvisInitialized = false;

protected:

	// Variables para el tracking relativo
	bool bHasTrackedFirstFrame = false;
	FVector InitialHipCenterAI; // Posici�n normalizada de la cadera en el primer frame

private:
	// --- MEDIA ---
	UPROPERTY() UMediaPlayer* MediaPlayer;
	UPROPERTY() UMediaTexture* MediaTexture;
	TArray<FColor> RawPixels;
	bool bIsReadingFrame = false;

	// NUEVO: Posici�n del "suelo" en component space
	FVector FloorRootLocal = FVector::ZeroVector;

	// NUEVO: Buffer de filtrado (posiciones anteriores por punto)
	TMap<EKinemotionPoint, FVector> PrevFilteredLocal;

	// --- NNE (IA) ---
	TSharedPtr<UE::NNE::IModelInstanceGPU> ModelInstance; // GPU -> CPU

	// Variable para guardar los �ltimos puntos calculados
	TMap<EKinemotionPoint, FVector> CachedWorldPoints;

	FVector InitialPelvisLocal = FVector::ZeroVector;
	bool bFirstFrameTracked = false;

	// Input Buffer
	TArray<float> InputTensor;
	int32 InputHeight = 384;
	int32 InputWidth = 288;

	// Output Buffers (SimCC)
	TArray<float> OutputTensorX;
	TArray<float> OutputTensorY;
	TArray<float> OutputTensorZ;
	TArray<UE::NNE::FTensorBindingCPU> InputBindings;     // GPU -> CPU
	TArray<UE::NNE::FTensorBindingCPU> OutputBindings;    // GPU -> CPU

	// --- LIVE LINK DIRECTO ---
	FGuid SourceGuid;
	FLiveLinkSubjectKey SubjectKey;
	bool bLiveLinkRegistered = false;
	TSharedPtr<FKinemotionLiveLinkSource> LiveLinkSource;  // YA LO TEN�AS, PERFECTO

	UFUNCTION()
	void CalibrateFromSkeleton();

	void CalculateEffectiveScale();

	FVector ApplySmoothing(EKinemotionPoint PointId, const FVector& NewPos);


	// --- FUNCIONES ---
	void InitMedia();
	void InitNNE();
	void SetupLiveLinkDirect();
	void RequestTextureRead();
	void OnFrameReadComplete();
	void PreProcessImage();
	void RunInference();
	void DecodeAndSend(const TArray<float>& X, const TArray<float>& Y, const TArray<float>& Z);
	float GetArgMax(const float* Buffer, int32 Size);
};