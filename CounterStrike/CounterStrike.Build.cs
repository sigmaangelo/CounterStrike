// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CounterStrike : ModuleRules
{
	public CounterStrike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "CounterStrike" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara", "NavigationSystem", "AIModule", "GameplayTasks", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
