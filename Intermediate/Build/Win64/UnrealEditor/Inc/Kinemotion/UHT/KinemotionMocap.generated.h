// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "KinemotionMocap.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
enum class EKinemotionPoint : uint8; 
#ifdef KINEMOTION_KinemotionMocap_generated_h
#error "KinemotionMocap.generated.h already included, missing '#pragma once' in KinemotionMocap.h"
#endif
#define KINEMOTION_KinemotionMocap_generated_h

#define FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_97_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execCalibrateFromSkeleton); \
	DECLARE_FUNCTION(execGetDetectedBodyPoints);


#define FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_97_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUKinemotionMocap(); \
	friend struct Z_Construct_UClass_UKinemotionMocap_Statics; \
public: \
	DECLARE_CLASS(UKinemotionMocap, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Kinemotion"), NO_API) \
	DECLARE_SERIALIZER(UKinemotionMocap)


#define FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_97_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UKinemotionMocap(UKinemotionMocap&&); \
	UKinemotionMocap(const UKinemotionMocap&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UKinemotionMocap); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UKinemotionMocap); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UKinemotionMocap) \
	NO_API virtual ~UKinemotionMocap();


#define FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_94_PROLOG
#define FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_97_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_97_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_97_INCLASS_NO_PURE_DECLS \
	FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h_97_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> KINEMOTION_API UClass* StaticClass<class UKinemotionMocap>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Proyectos_ProyectosUnreal_KinemotionPlugin_Plugins_Kinemotion_Source_Kinemotion_Public_KinemotionMocap_h


#define FOREACH_ENUM_EKINEMOTIONPOINT(op) \
	op(EKinemotionPoint::Nose) \
	op(EKinemotionPoint::LeftShoulder) \
	op(EKinemotionPoint::RightShoulder) \
	op(EKinemotionPoint::LeftElbow) \
	op(EKinemotionPoint::RightElbow) \
	op(EKinemotionPoint::LeftWrist) \
	op(EKinemotionPoint::RightWrist) \
	op(EKinemotionPoint::LeftHip) \
	op(EKinemotionPoint::RightHip) \
	op(EKinemotionPoint::LeftKnee) \
	op(EKinemotionPoint::RightKnee) \
	op(EKinemotionPoint::LeftAnkle) \
	op(EKinemotionPoint::RightAnkle) \
	op(EKinemotionPoint::HipCenter) \
	op(EKinemotionPoint::NeckBase) \
	op(EKinemotionPoint::SpineMid) \
	op(EKinemotionPoint::LeftClavicle) \
	op(EKinemotionPoint::RightClavicle) 

enum class EKinemotionPoint : uint8;
template<> struct TIsUEnumClass<EKinemotionPoint> { enum { Value = true }; };
template<> KINEMOTION_API UEnum* StaticEnum<EKinemotionPoint>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
