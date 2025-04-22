// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FCSpawner : ModuleRules
{
	public FCSpawner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"FCGlobal",
		});

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd",
			});
		}
	}
}