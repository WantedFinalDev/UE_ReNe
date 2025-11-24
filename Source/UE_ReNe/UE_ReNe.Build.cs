// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_ReNe : ModuleRules
{
	public UE_ReNe(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore", 
			"OnlineSubsystem", 
			"OnlineSubsystemSteam",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			
		});

		PublicIncludePaths.AddRange(new string[] {
			"UE_ReNe",
			"UE_ReNe/Variant_Horror",
			"UE_ReNe/Variant_Horror/UI",
			"UE_ReNe/Variant_Shooter",
			"UE_ReNe/Variant_Shooter/AI",
			"UE_ReNe/Variant_Shooter/UI",
			"UE_ReNe/Variant_Shooter/Weapons",
		});
		
		// 모듈 포함 경로 정리: Public/Private 및 모듈 루트
		PublicIncludePaths.AddRange(new string[]
		{
			System.IO.Path.Combine(ModuleDirectory, "Public"),
		});

		PrivateIncludePaths.AddRange(new string[]
		{
			System.IO.Path.Combine(ModuleDirectory, "Private"),
			ModuleDirectory
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
