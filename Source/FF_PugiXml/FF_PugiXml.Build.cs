// Some copyright should be here...

using System;
using System.IO;
using UnrealBuildTool;

public class FF_PugiXml : ModuleRules
{
	public FF_PugiXml(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnableExceptions = true;
        bUseRTTI = true;

		// If you need to use MaterialX or Serialization utils, you need to disable this and activate "MaterialX" from PrivateDependencyModuleNames
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty", "pugixml"));

        // for pugixml wchar support
        // PrivateDefinitions.Add("PUGIXML_WCHAR_MODE"); 

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                //"MaterialX",	// for PugiXML
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
