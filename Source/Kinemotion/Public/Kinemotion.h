// Copyright (c) Victor Rivas Perez. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"
#include "Modules/ModuleManager.h"

/**
 * Plugin-wide log category.
 *
 * Kinemotion spans three subsystems that fail in different ways - media capture, neural
 * inference and Live Link - so its output needs to be filterable on its own. Sharing LogTemp
 * with the rest of the engine makes that impossible.
 */
DECLARE_LOG_CATEGORY_EXTERN(LogKinemotion, Log, All);

class FKinemotionModule : public IModuleInterface
{
public:
	//~ Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~ End IModuleInterface
};
