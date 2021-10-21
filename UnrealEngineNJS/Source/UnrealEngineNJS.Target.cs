//
// Created by TekuConcept on October 20, 2021
//

using UnrealBuildTool;
using System.Collections.Generic;

public class UnrealEngineNJSTarget : TargetRules
{
	public UnrealEngineNJSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "UnrealEngineNJS" } );
	}
}
