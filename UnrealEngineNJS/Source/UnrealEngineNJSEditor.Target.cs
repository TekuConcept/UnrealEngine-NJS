//
// Created by TekuConcept on October 20, 2021
//

using UnrealBuildTool;
using System.Collections.Generic;

public class UnrealEngineNJSEditorTarget : TargetRules
{
	public UnrealEngineNJSEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "UnrealEngineNJS" } );
	}
}
