// Some copyright should be here...

using System;
using System.IO;
using UnrealBuildTool;

public class FF_PugiXml : ModuleRules
{
	public FF_PugiXml(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        string Location_PugiXml = "../Source/FF_PugiXml/ThirdParty/pugixml";
        PrivateIncludePaths.Add(Location_PugiXml);

        if (UnrealTargetPlatform.Win64 == Target.Platform)
        {
            string Location_Libdeflate = "../Source/FF_PugiXml/ThirdParty/libdeflate/Windows/include";
            PrivateIncludePaths.Add(Location_Libdeflate);

            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "ThirdParty", "libdeflate", "Windows", "lib", "deflatestatic.lib"));
        }

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
