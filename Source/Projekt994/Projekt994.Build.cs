// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Projekt994 : ModuleRules
{
	public Projekt994(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Http", "Json", "GameplayTasks", "JsonUtilities","AIModule" });
	}
}
