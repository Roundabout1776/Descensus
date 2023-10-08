using UnrealBuildTool;

public class Descensus : ModuleRules
{
	public Descensus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara",
			"GameplayAbilities"
		});
		PrivateDependencyModuleNames.AddRange(new[] { "Slate", "SlateCore", "GameplayTags", "GameplayTasks" });
		PublicIncludePaths.AddRange(new string[] {"Descensus"});
		PrivateIncludePaths.AddRange(new string[] { });
		PublicIncludePathModuleNames.AddRange(new string[] { });
		PrivateIncludePathModuleNames.AddRange(new string[] { });
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new [] {"UnrealEd"});
		}
	}
}