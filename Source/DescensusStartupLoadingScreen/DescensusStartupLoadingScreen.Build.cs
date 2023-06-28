using UnrealBuildTool;

public class DescensusStartupLoadingScreen : ModuleRules
{
    public DescensusStartupLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
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
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "PreLoadScreen",
                "PreLoadScreenMoviePlayer",
                "Descensus"
            }
        );
    }
}