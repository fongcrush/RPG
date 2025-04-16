// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FongcrushSpawner : ModuleRules
{
	public FongcrushSpawner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"ComponentVisualizers",
		});

		if (!Target.bBuildEditor) return;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"FongcrushGlobal"
		});
			
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"UnrealEd",
		});
	}
}