using UnrealBuildTool;

public class FCSpawnerEditor : ModuleRules
{
    public FCSpawnerEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[]
        {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
			    "ComponentVisualizers",
			    "DeveloperSettings",
			    "UnrealEd",
			    "FCGlobal",
                "FCSpawner",
                "EditorFramework",
        });
    }
}