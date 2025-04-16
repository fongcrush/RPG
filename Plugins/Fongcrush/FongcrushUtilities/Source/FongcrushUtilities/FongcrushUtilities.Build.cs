// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FongcrushUtilities : ModuleRules
{
	public FongcrushUtilities(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"EnhancedInput",
				"GameplayAbilities",
				"GameplayTags",
				"CoreUObject",
				"Engine",
			}
		);
	}
}
