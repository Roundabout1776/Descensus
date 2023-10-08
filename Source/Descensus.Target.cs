using UnrealBuildTool;

public class DescensusTarget : TargetRules
{
	public DescensusTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "Descensus", "DescensusStartupLoadingScreen" } );
	}
}
