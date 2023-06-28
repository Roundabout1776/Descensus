using UnrealBuildTool;

public class DescensusEditor : ModuleRules
{
    public DescensusEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new []
            {
                "Core", "AssetTools"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new []
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Descensus",
                "UnrealEd",
                "InputCore"
            }
        );
    }
}