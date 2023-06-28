using UnrealBuildTool;
using System.Collections.Generic;

public class DescensusEditorTarget : TargetRules
{
	public DescensusEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "Descensus", "DescensusEditor", "DescensusStartupLoadingScreen" } );
	}
}
