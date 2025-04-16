// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FongcrushTestEditor : ModuleRules
{
	public FongcrushTestEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"CoreUObject",
				"Slate",
				"SlateCore",
				"InputCore",
				"LevelEditor",
				"BlueprintGraph",
				"AnimGraph",
				"UnrealEd",
				"AssetTools",
				"EditorWidgets",
				"ComponentVisualizers",
				"EditorFramework"
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"EditorStyle"
			}
		);
	}
}