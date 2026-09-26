// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LavaHuangGress : ModuleRules
{
	public LavaHuangGress(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"LavaHuangGress",
			"LavaHuangGress/Variant_Platforming",
			"LavaHuangGress/Variant_Platforming/Animation",
			"LavaHuangGress/Variant_Combat",
			"LavaHuangGress/Variant_Combat/AI",
			"LavaHuangGress/Variant_Combat/Animation",
			"LavaHuangGress/Variant_Combat/Gameplay",
			"LavaHuangGress/Variant_Combat/Interfaces",
			"LavaHuangGress/Variant_Combat/UI",
			"LavaHuangGress/Variant_SideScrolling",
			"LavaHuangGress/Variant_SideScrolling/AI",
			"LavaHuangGress/Variant_SideScrolling/Gameplay",
			"LavaHuangGress/Variant_SideScrolling/Interfaces",
			"LavaHuangGress/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
