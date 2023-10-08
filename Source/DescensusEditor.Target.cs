using UnrealBuildTool;

public class DescensusEditorTarget : TargetRules
{
	public DescensusEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "Descensus", "DescensusEditor", "DescensusStartupLoadingScreen" } );
	}
}
