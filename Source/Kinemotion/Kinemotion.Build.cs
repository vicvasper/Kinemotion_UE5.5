// Copyright (c) Victor Rivas Perez. All Rights Reserved.

using UnrealBuildTool;

public class Kinemotion : ModuleRules
{
	public Kinemotion(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// Public: reachable through this module's public headers, so any module depending on
		// Kinemotion needs them too. NNE and LiveLinkInterface belong here rather than in the
		// private list because KinemotionMocap.h and KinemotionLiveLinkSource.h expose their
		// types directly - listing them as private compiles today only because nothing depends
		// on this module yet.
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"NNE",
			"LiveLinkInterface",
		});

		// Private: referenced only from the implementation.
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"LiveLinkAnimationCore",
			"Media",
			"MediaAssets",
			"RHI",
			"RenderCore",
		});

		// Removed: "Slate" and "SlateCore" (runtime module, no UI) and
		// "LiveLinkMessageBusFramework" (frames are pushed straight to the client rather than
		// over the message bus). All three were linked without a single reference.
	}
}
