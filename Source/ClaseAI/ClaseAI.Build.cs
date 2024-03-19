// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ClaseAI : ModuleRules
{
	public ClaseAI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
        
        PublicIncludePaths.AddRange(new string[]
        {
	        "ClaseAI"
        });
	}
}
