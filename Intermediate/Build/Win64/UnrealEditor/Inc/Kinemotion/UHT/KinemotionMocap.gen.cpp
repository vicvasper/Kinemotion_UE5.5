// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Kinemotion/Public/KinemotionMocap.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeKinemotionMocap() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
KINEMOTION_API UClass* Z_Construct_UClass_UKinemotionMocap();
KINEMOTION_API UClass* Z_Construct_UClass_UKinemotionMocap_NoRegister();
KINEMOTION_API UEnum* Z_Construct_UEnum_Kinemotion_EKinemotionPoint();
MEDIAASSETS_API UClass* Z_Construct_UClass_UMediaPlayer_NoRegister();
MEDIAASSETS_API UClass* Z_Construct_UClass_UMediaTexture_NoRegister();
NNE_API UClass* Z_Construct_UClass_UNNEModelData_NoRegister();
UPackage* Z_Construct_UPackage__Script_Kinemotion();
// End Cross Module References

// Begin Enum EKinemotionPoint
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EKinemotionPoint;
static UEnum* EKinemotionPoint_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EKinemotionPoint.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EKinemotionPoint.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_Kinemotion_EKinemotionPoint, (UObject*)Z_Construct_UPackage__Script_Kinemotion(), TEXT("EKinemotionPoint"));
	}
	return Z_Registration_Info_UEnum_EKinemotionPoint.OuterSingleton;
}
template<> KINEMOTION_API UEnum* StaticEnum<EKinemotionPoint>()
{
	return EKinemotionPoint_StaticEnum();
}
struct Z_Construct_UEnum_Kinemotion_EKinemotionPoint_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "HipCenter.Comment", "// --- PUNTOS VIRTUALES (Calculados) ---\n" },
		{ "HipCenter.Name", "EKinemotionPoint::HipCenter" },
		{ "HipCenter.ToolTip", "--- PUNTOS VIRTUALES (Calculados) ---" },
		{ "LeftAnkle.Name", "EKinemotionPoint::LeftAnkle" },
		{ "LeftClavicle.Name", "EKinemotionPoint::LeftClavicle" },
		{ "LeftElbow.Name", "EKinemotionPoint::LeftElbow" },
		{ "LeftHip.Name", "EKinemotionPoint::LeftHip" },
		{ "LeftKnee.Name", "EKinemotionPoint::LeftKnee" },
		{ "LeftShoulder.Name", "EKinemotionPoint::LeftShoulder" },
		{ "LeftWrist.Name", "EKinemotionPoint::LeftWrist" },
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
		{ "NeckBase.Name", "EKinemotionPoint::NeckBase" },
		{ "Nose.Name", "EKinemotionPoint::Nose" },
		{ "RightAnkle.Name", "EKinemotionPoint::RightAnkle" },
		{ "RightClavicle.Name", "EKinemotionPoint::RightClavicle" },
		{ "RightElbow.Name", "EKinemotionPoint::RightElbow" },
		{ "RightHip.Name", "EKinemotionPoint::RightHip" },
		{ "RightKnee.Name", "EKinemotionPoint::RightKnee" },
		{ "RightShoulder.Name", "EKinemotionPoint::RightShoulder" },
		{ "RightWrist.Name", "EKinemotionPoint::RightWrist" },
		{ "SpineMid.Name", "EKinemotionPoint::SpineMid" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EKinemotionPoint::Nose", (int64)EKinemotionPoint::Nose },
		{ "EKinemotionPoint::LeftShoulder", (int64)EKinemotionPoint::LeftShoulder },
		{ "EKinemotionPoint::RightShoulder", (int64)EKinemotionPoint::RightShoulder },
		{ "EKinemotionPoint::LeftElbow", (int64)EKinemotionPoint::LeftElbow },
		{ "EKinemotionPoint::RightElbow", (int64)EKinemotionPoint::RightElbow },
		{ "EKinemotionPoint::LeftWrist", (int64)EKinemotionPoint::LeftWrist },
		{ "EKinemotionPoint::RightWrist", (int64)EKinemotionPoint::RightWrist },
		{ "EKinemotionPoint::LeftHip", (int64)EKinemotionPoint::LeftHip },
		{ "EKinemotionPoint::RightHip", (int64)EKinemotionPoint::RightHip },
		{ "EKinemotionPoint::LeftKnee", (int64)EKinemotionPoint::LeftKnee },
		{ "EKinemotionPoint::RightKnee", (int64)EKinemotionPoint::RightKnee },
		{ "EKinemotionPoint::LeftAnkle", (int64)EKinemotionPoint::LeftAnkle },
		{ "EKinemotionPoint::RightAnkle", (int64)EKinemotionPoint::RightAnkle },
		{ "EKinemotionPoint::HipCenter", (int64)EKinemotionPoint::HipCenter },
		{ "EKinemotionPoint::NeckBase", (int64)EKinemotionPoint::NeckBase },
		{ "EKinemotionPoint::SpineMid", (int64)EKinemotionPoint::SpineMid },
		{ "EKinemotionPoint::LeftClavicle", (int64)EKinemotionPoint::LeftClavicle },
		{ "EKinemotionPoint::RightClavicle", (int64)EKinemotionPoint::RightClavicle },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_Kinemotion_EKinemotionPoint_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_Kinemotion,
	nullptr,
	"EKinemotionPoint",
	"EKinemotionPoint",
	Z_Construct_UEnum_Kinemotion_EKinemotionPoint_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_Kinemotion_EKinemotionPoint_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_Kinemotion_EKinemotionPoint_Statics::Enum_MetaDataParams), Z_Construct_UEnum_Kinemotion_EKinemotionPoint_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_Kinemotion_EKinemotionPoint()
{
	if (!Z_Registration_Info_UEnum_EKinemotionPoint.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EKinemotionPoint.InnerSingleton, Z_Construct_UEnum_Kinemotion_EKinemotionPoint_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EKinemotionPoint.InnerSingleton;
}
// End Enum EKinemotionPoint

// Begin Class UKinemotionMocap Function CalibrateFromSkeleton
struct Z_Construct_UFunction_UKinemotionMocap_CalibrateFromSkeleton_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// YA LO TEN\xef\xbf\xbd""AS, PERFECTO\n" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "YA LO TEN\xef\xbf\xbd""AS, PERFECTO" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UKinemotionMocap_CalibrateFromSkeleton_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKinemotionMocap, nullptr, "CalibrateFromSkeleton", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UKinemotionMocap_CalibrateFromSkeleton_Statics::Function_MetaDataParams), Z_Construct_UFunction_UKinemotionMocap_CalibrateFromSkeleton_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UKinemotionMocap_CalibrateFromSkeleton()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UKinemotionMocap_CalibrateFromSkeleton_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UKinemotionMocap::execCalibrateFromSkeleton)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->CalibrateFromSkeleton();
	P_NATIVE_END;
}
// End Class UKinemotionMocap Function CalibrateFromSkeleton

// Begin Class UKinemotionMocap Function GetDetectedBodyPoints
struct Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics
{
	struct KinemotionMocap_eventGetDetectedBodyPoints_Parms
	{
		TMap<EKinemotionPoint,FVector> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Kinemotion AI" },
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_ValueProp;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Key_KeyProp_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::NewProp_ReturnValue_ValueProp = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::NewProp_ReturnValue_Key_KeyProp_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::NewProp_ReturnValue_Key_KeyProp = { "ReturnValue_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_Kinemotion_EKinemotionPoint, METADATA_PARAMS(0, nullptr) }; // 2306000641
const UECodeGen_Private::FMapPropertyParams Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(KinemotionMocap_eventGetDetectedBodyPoints_Parms, ReturnValue), EMapPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 2306000641
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::NewProp_ReturnValue_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::NewProp_ReturnValue_Key_KeyProp_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::NewProp_ReturnValue_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKinemotionMocap, nullptr, "GetDetectedBodyPoints", nullptr, nullptr, Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::PropPointers), sizeof(Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::KinemotionMocap_eventGetDetectedBodyPoints_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::Function_MetaDataParams), Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::KinemotionMocap_eventGetDetectedBodyPoints_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UKinemotionMocap::execGetDetectedBodyPoints)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TMap<EKinemotionPoint,FVector>*)Z_Param__Result=P_THIS->GetDetectedBodyPoints();
	P_NATIVE_END;
}
// End Class UKinemotionMocap Function GetDetectedBodyPoints

// Begin Class UKinemotionMocap
void UKinemotionMocap::StaticRegisterNativesUKinemotionMocap()
{
	UClass* Class = UKinemotionMocap::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "CalibrateFromSkeleton", &UKinemotionMocap::execCalibrateFromSkeleton },
		{ "GetDetectedBodyPoints", &UKinemotionMocap::execGetDetectedBodyPoints },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UKinemotionMocap);
UClass* Z_Construct_UClass_UKinemotionMocap_NoRegister()
{
	return UKinemotionMocap::StaticClass();
}
struct Z_Construct_UClass_UKinemotionMocap_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// ============================================\n// COMPONENTE PRINCIPAL\n// ============================================\n" },
#endif
		{ "IncludePath", "KinemotionMocap.h" },
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "COMPONENTE PRINCIPAL" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ModelData_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// --- CONFIGURACI\xef\xbf\xbdN ---\n" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "--- CONFIGURACI\xef\xbf\xbdN ---" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SubjectName_MetaData[] = {
		{ "Category", "Kinemotion" },
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CameraIndex_MetaData[] = {
		{ "Category", "Kinemotion" },
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WorldScale_MetaData[] = {
		{ "Category", "Kinemotion" },
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GlobalScaleFactor_MetaData[] = {
		{ "Category", "Kinemotion|Scale" },
		{ "ClampMax", "10.0" },
		{ "ClampMin", "0.1" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Factor de escala global del stickman (1.0 = tama\xef\xbf\xbdo del skeleton, 2.0 = doble) */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Factor de escala global del stickman (1.0 = tama\xef\xbf\xbdo del skeleton, 2.0 = doble)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScaleX_MetaData[] = {
		{ "Category", "Kinemotion|Scale" },
		{ "ClampMax", "5.0" },
		{ "ClampMin", "0.1" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Escala en X (Profundidad/Adelante-Atr\xef\xbf\xbds) */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Escala en X (Profundidad/Adelante-Atr\xef\xbf\xbds)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScaleY_MetaData[] = {
		{ "Category", "Kinemotion|Scale" },
		{ "ClampMax", "5.0" },
		{ "ClampMin", "0.1" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Escala en Y (Ancho/Izquierda-Derecha) - Aumenta para mejor movimiento lateral */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Escala en Y (Ancho/Izquierda-Derecha) - Aumenta para mejor movimiento lateral" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScaleZ_MetaData[] = {
		{ "Category", "Kinemotion|Scale" },
		{ "ClampMax", "5.0" },
		{ "ClampMin", "0.1" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Escala en Z (Altura/Arriba-Abajo) */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Escala en Z (Altura/Arriba-Abajo)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAutoFixProportions_MetaData[] = {
		{ "Category", "Kinemotion|Scale" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Auto-corregir proporciones aplastadas (recomendado) */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Auto-corregir proporciones aplastadas (recomendado)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bDebugCalibration_MetaData[] = {
		{ "Category", "Kinemotion|Debug" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Mostrar informaci\xef\xbf\xbdn de calibraci\xef\xbf\xbdn en logs */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Mostrar informaci\xef\xbf\xbdn de calibraci\xef\xbf\xbdn en logs" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bIsCalibrated_MetaData[] = {
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAutoCalibrate_MetaData[] = {
		{ "Category", "Kinemotion|Calibration" },
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseFloorAsRoot_MetaData[] = {
		{ "Category", "Kinemotion|Root" },
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bPelvisFree_MetaData[] = {
		{ "Category", "Kinemotion|Root" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Si true, la pelvis puede subir/bajar libremente (para agacharse).\n\x09\x09Si false, la pelvis est\xef\xbf\xbd fija respecto al root. */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Si true, la pelvis puede subir/bajar libremente (para agacharse).\n              Si false, la pelvis est\xef\xbf\xbd fija respecto al root." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FloorZOffset_MetaData[] = {
		{ "Category", "Kinemotion|Root" },
		{ "ClampMax", "50.0" },
		{ "ClampMin", "-50.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Offset vertical del suelo (por si el mesh tiene suelas, etc.). */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Offset vertical del suelo (por si el mesh tiene suelas, etc.)." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StickmanScale_MetaData[] = {
		{ "Category", "Kinemotion|Scale" },
		{ "ClampMax", "10.0" },
		{ "ClampMin", "0.1" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Multiplicador de escala del stickman (1.0 = tama\xef\xbf\xbdo del skeleton, 2.0 = doble) */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Multiplicador de escala del stickman (1.0 = tama\xef\xbf\xbdo del skeleton, 2.0 = doble)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bScaleRelativeToSkeleton_MetaData[] = {
		{ "Category", "Kinemotion|Scale" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Usar altura del skeleton como referencia (true) o tama\xef\xbf\xbdo absoluto en cm (false) */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Usar altura del skeleton como referencia (true) o tama\xef\xbf\xbdo absoluto en cm (false)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReferenceSizeCM_MetaData[] = {
		{ "Category", "Kinemotion|Scale" },
		{ "ClampMin", "10.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Si bScaleRelativeToSkeleton = false, usar este tama\xef\xbf\xbdo base en cm */" },
#endif
		{ "EditCondition", "!bScaleRelativeToSkeleton" },
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Si bScaleRelativeToSkeleton = false, usar este tama\xef\xbf\xbdo base en cm" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SmoothingAlpha_MetaData[] = {
		{ "Category", "Kinemotion|Filter" },
		{ "ClampMax", "0.95" },
		{ "ClampMin", "0.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Fuerza de suavizado [0..1]. 0 = sin suavizado, 0.3 = recomendado, 0.5+ = muy suave pero con lag. */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Fuerza de suavizado [0..1]. 0 = sin suavizado, 0.3 = recomendado, 0.5+ = muy suave pero con lag." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DeadzoneCM_MetaData[] = {
		{ "Category", "Kinemotion|Filter" },
		{ "ClampMax", "5.0" },
		{ "ClampMin", "0.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Zona muerta (cm). Movimientos menores a este valor se ignoran para reducir micro-jitter. */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Zona muerta (cm). Movimientos menores a este valor se ignoran para reducir micro-jitter." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxStepCM_MetaData[] = {
		{ "Category", "Kinemotion|Filter" },
		{ "ClampMax", "50.0" },
		{ "ClampMin", "1.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** M\xef\xbf\xbdximo desplazamiento por frame (cm). Limita picos/saltos. */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "M\xef\xbf\xbdximo desplazamiento por frame (cm). Limita picos/saltos." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableSmoothing_MetaData[] = {
		{ "Category", "Kinemotion|Filter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Activar/desactivar filtro. */" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Activar/desactivar filtro." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bShowDebug_MetaData[] = {
		{ "Category", "Kinemotion|Debug" },
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MediaPlayer_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// --- MEDIA ---\n" },
#endif
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "--- MEDIA ---" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MediaTexture_MetaData[] = {
		{ "ModuleRelativePath", "Public/KinemotionMocap.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ModelData;
	static const UECodeGen_Private::FStrPropertyParams NewProp_SubjectName;
	static const UECodeGen_Private::FIntPropertyParams NewProp_CameraIndex;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_WorldScale;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_GlobalScaleFactor;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ScaleX;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ScaleY;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ScaleZ;
	static void NewProp_bAutoFixProportions_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAutoFixProportions;
	static void NewProp_bDebugCalibration_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bDebugCalibration;
	static void NewProp_bIsCalibrated_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bIsCalibrated;
	static void NewProp_bAutoCalibrate_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAutoCalibrate;
	static void NewProp_bUseFloorAsRoot_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseFloorAsRoot;
	static void NewProp_bPelvisFree_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bPelvisFree;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FloorZOffset;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_StickmanScale;
	static void NewProp_bScaleRelativeToSkeleton_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bScaleRelativeToSkeleton;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReferenceSizeCM;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_SmoothingAlpha;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_DeadzoneCM;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxStepCM;
	static void NewProp_bEnableSmoothing_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableSmoothing;
	static void NewProp_bShowDebug_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bShowDebug;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MediaPlayer;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MediaTexture;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UKinemotionMocap_CalibrateFromSkeleton, "CalibrateFromSkeleton" }, // 2352084426
		{ &Z_Construct_UFunction_UKinemotionMocap_GetDetectedBodyPoints, "GetDetectedBodyPoints" }, // 2843184664
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UKinemotionMocap>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_ModelData = { "ModelData", nullptr, (EPropertyFlags)0x0114000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, ModelData), Z_Construct_UClass_UNNEModelData_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ModelData_MetaData), NewProp_ModelData_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_SubjectName = { "SubjectName", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, SubjectName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SubjectName_MetaData), NewProp_SubjectName_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_CameraIndex = { "CameraIndex", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, CameraIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CameraIndex_MetaData), NewProp_CameraIndex_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_WorldScale = { "WorldScale", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, WorldScale), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WorldScale_MetaData), NewProp_WorldScale_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_GlobalScaleFactor = { "GlobalScaleFactor", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, GlobalScaleFactor), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GlobalScaleFactor_MetaData), NewProp_GlobalScaleFactor_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_ScaleX = { "ScaleX", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, ScaleX), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScaleX_MetaData), NewProp_ScaleX_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_ScaleY = { "ScaleY", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, ScaleY), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScaleY_MetaData), NewProp_ScaleY_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_ScaleZ = { "ScaleZ", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, ScaleZ), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScaleZ_MetaData), NewProp_ScaleZ_MetaData) };
void Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bAutoFixProportions_SetBit(void* Obj)
{
	((UKinemotionMocap*)Obj)->bAutoFixProportions = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bAutoFixProportions = { "bAutoFixProportions", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UKinemotionMocap), &Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bAutoFixProportions_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAutoFixProportions_MetaData), NewProp_bAutoFixProportions_MetaData) };
void Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bDebugCalibration_SetBit(void* Obj)
{
	((UKinemotionMocap*)Obj)->bDebugCalibration = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bDebugCalibration = { "bDebugCalibration", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UKinemotionMocap), &Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bDebugCalibration_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDebugCalibration_MetaData), NewProp_bDebugCalibration_MetaData) };
void Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bIsCalibrated_SetBit(void* Obj)
{
	((UKinemotionMocap*)Obj)->bIsCalibrated = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bIsCalibrated = { "bIsCalibrated", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UKinemotionMocap), &Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bIsCalibrated_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bIsCalibrated_MetaData), NewProp_bIsCalibrated_MetaData) };
void Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bAutoCalibrate_SetBit(void* Obj)
{
	((UKinemotionMocap*)Obj)->bAutoCalibrate = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bAutoCalibrate = { "bAutoCalibrate", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UKinemotionMocap), &Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bAutoCalibrate_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAutoCalibrate_MetaData), NewProp_bAutoCalibrate_MetaData) };
void Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bUseFloorAsRoot_SetBit(void* Obj)
{
	((UKinemotionMocap*)Obj)->bUseFloorAsRoot = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bUseFloorAsRoot = { "bUseFloorAsRoot", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UKinemotionMocap), &Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bUseFloorAsRoot_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseFloorAsRoot_MetaData), NewProp_bUseFloorAsRoot_MetaData) };
void Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bPelvisFree_SetBit(void* Obj)
{
	((UKinemotionMocap*)Obj)->bPelvisFree = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bPelvisFree = { "bPelvisFree", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UKinemotionMocap), &Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bPelvisFree_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bPelvisFree_MetaData), NewProp_bPelvisFree_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_FloorZOffset = { "FloorZOffset", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, FloorZOffset), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FloorZOffset_MetaData), NewProp_FloorZOffset_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_StickmanScale = { "StickmanScale", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, StickmanScale), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StickmanScale_MetaData), NewProp_StickmanScale_MetaData) };
void Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bScaleRelativeToSkeleton_SetBit(void* Obj)
{
	((UKinemotionMocap*)Obj)->bScaleRelativeToSkeleton = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bScaleRelativeToSkeleton = { "bScaleRelativeToSkeleton", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UKinemotionMocap), &Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bScaleRelativeToSkeleton_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bScaleRelativeToSkeleton_MetaData), NewProp_bScaleRelativeToSkeleton_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_ReferenceSizeCM = { "ReferenceSizeCM", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, ReferenceSizeCM), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReferenceSizeCM_MetaData), NewProp_ReferenceSizeCM_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_SmoothingAlpha = { "SmoothingAlpha", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, SmoothingAlpha), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SmoothingAlpha_MetaData), NewProp_SmoothingAlpha_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_DeadzoneCM = { "DeadzoneCM", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, DeadzoneCM), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DeadzoneCM_MetaData), NewProp_DeadzoneCM_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_MaxStepCM = { "MaxStepCM", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, MaxStepCM), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxStepCM_MetaData), NewProp_MaxStepCM_MetaData) };
void Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bEnableSmoothing_SetBit(void* Obj)
{
	((UKinemotionMocap*)Obj)->bEnableSmoothing = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bEnableSmoothing = { "bEnableSmoothing", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UKinemotionMocap), &Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bEnableSmoothing_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableSmoothing_MetaData), NewProp_bEnableSmoothing_MetaData) };
void Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bShowDebug_SetBit(void* Obj)
{
	((UKinemotionMocap*)Obj)->bShowDebug = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bShowDebug = { "bShowDebug", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UKinemotionMocap), &Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bShowDebug_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bShowDebug_MetaData), NewProp_bShowDebug_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_MediaPlayer = { "MediaPlayer", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, MediaPlayer), Z_Construct_UClass_UMediaPlayer_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MediaPlayer_MetaData), NewProp_MediaPlayer_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_MediaTexture = { "MediaTexture", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UKinemotionMocap, MediaTexture), Z_Construct_UClass_UMediaTexture_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MediaTexture_MetaData), NewProp_MediaTexture_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UKinemotionMocap_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_ModelData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_SubjectName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_CameraIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_WorldScale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_GlobalScaleFactor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_ScaleX,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_ScaleY,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_ScaleZ,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bAutoFixProportions,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bDebugCalibration,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bIsCalibrated,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bAutoCalibrate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bUseFloorAsRoot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bPelvisFree,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_FloorZOffset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_StickmanScale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bScaleRelativeToSkeleton,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_ReferenceSizeCM,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_SmoothingAlpha,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_DeadzoneCM,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_MaxStepCM,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bEnableSmoothing,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_bShowDebug,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_MediaPlayer,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UKinemotionMocap_Statics::NewProp_MediaTexture,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UKinemotionMocap_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UKinemotionMocap_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_Kinemotion,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UKinemotionMocap_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UKinemotionMocap_Statics::ClassParams = {
	&UKinemotionMocap::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UKinemotionMocap_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UKinemotionMocap_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UKinemotionMocap_Statics::Class_MetaDataParams), Z_Construct_UClass_UKinemotionMocap_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UKinemotionMocap()
{
	if (!Z_Registration_Info_UClass_UKinemotionMocap.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UKinemotionMocap.OuterSingleton, Z_Construct_UClass_UKinemotionMocap_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UKinemotionMocap.OuterSingleton;
}
template<> KINEMOTION_API UClass* StaticClass<UKinemotionMocap>()
{
	return UKinemotionMocap::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UKinemotionMocap);
UKinemotionMocap::~UKinemotionMocap() {}
// End Class UKinemotionMocap

// Begin Registration
struct Z_CompiledInDeferFile_FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EKinemotionPoint_StaticEnum, TEXT("EKinemotionPoint"), &Z_Registration_Info_UEnum_EKinemotionPoint, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2306000641U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UKinemotionMocap, UKinemotionMocap::StaticClass, TEXT("UKinemotionMocap"), &Z_Registration_Info_UClass_UKinemotionMocap, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UKinemotionMocap), 3062345728U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_2398289768(TEXT("/Script/Kinemotion"),
	Z_CompiledInDeferFile_FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_Statics::ClassInfo),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
