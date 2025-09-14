// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LambdaSnailInventorySystem : ModuleRules
{
	public LambdaSnailInventorySystem(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"GameplayAbilities",
				"GameplayTags",
				"UMG"
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
				"DeveloperSettings"
				// ... add private dependencies that you statically link with here ...	
			}
		);

		if (Target.bBuildEditor) PrivateDependencyModuleNames.Add("UnrealEd");


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}