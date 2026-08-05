// Copyright (c) Victor Rivas Perez. All Rights Reserved.

#include "Kinemotion.h"

DEFINE_LOG_CATEGORY(LogKinemotion);

#define LOCTEXT_NAMESPACE "FKinemotionModule"

void FKinemotionModule::StartupModule()
{
	// Live Link is reached through the modular-feature registry at runtime rather than linked
	// against, so nothing forces these modules to be resident by the time a component's
	// BeginPlay runs. Loading them here removes that ordering dependency.
	FModuleManager::Get().LoadModule(TEXT("LiveLink"));
	FModuleManager::Get().LoadModule(TEXT("LiveLinkAnimationCore"));
}

void FKinemotionModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKinemotionModule, Kinemotion)
