// Copyright Epic Games, Inc. All Rights Reserved.

#include "Kinemotion.h"

#define LOCTEXT_NAMESPACE "FKinemotionModule"

void FKinemotionModule::StartupModule()
{
	FModuleManager::Get().LoadModule(TEXT("LiveLink"));
	FModuleManager::Get().LoadModule(TEXT("LiveLinkAnimationCore"));
}

void FKinemotionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKinemotionModule, Kinemotion)