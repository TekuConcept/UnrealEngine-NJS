//
// Created by TekuConcept on October 20, 2021
//

using System.IO;
using UnrealBuildTool;

public class EmbeddedNodeJS : ModuleRules
{
	public EmbeddedNodeJS(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableUndefinedIdentifierWarnings = false;


		PublicDefinitions.AddRange(new[]
		{
			"USING_V8_SHARED=1",
			"V8_HAS_ATTRIBUTE_ALWAYS_INLINE=0",
			"V8_HAS_ATTRIBUTE_NOINLINE=0",
			"V8_HAS_ATTRIBUTE_NONNULL=0",
			"V8_HAS_BUILTIN_ASSUME_ALIGNED=0",
			"V8_HAS_BUILTIN_EXPECT=0",
			"V8_HAS_ATTRIBUTE_WARN_UNUSED_RESULT=0",
			"V8_HAS_CPP_ATTRIBUTE_NODISCARD=0"
		});


		PublicIncludePaths.AddRange(new[]
		{
			Path.Combine(ModuleDirectory, "../ThirdParty/Public/node"),
			Path.Combine(ModuleDirectory, "../ThirdParty/Public/uv"),
			Path.Combine(ModuleDirectory, "../ThirdParty/Public/v8")
		});


		PublicDependencyModuleNames.AddRange(new[]
			{
				"Core",
				"NodeJSExtension",
				"Projects",
				"Engine"
			}
		);
	}
}
