// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class RPG : ModuleRules
{
	public RPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"EditorScriptingUtilities",
			"DataTableEditor",
			"AssetRegistry",
			"Settings",
			"FCUtilities",
			"FCSpawner",
			"FCSpawnerEditor",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"EnhancedInput",
			"Niagara",
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