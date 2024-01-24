using UnrealBuildTool;

public class GGJ24 : ModuleRules
{
	public GGJ24(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"SlateCore",
			"CoreUObject",
			"Engine",
			"InputCore",
			"GameplayTags",
			"GameFeatures"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"EnhancedInput", 
			"CommonUI",
			"CommonInput",
			"AudioMixer",
			"AudioModulation",
			"DeveloperSettings",
			"PropertyPath", 
			"ModularGameplay"
		});
	}
}