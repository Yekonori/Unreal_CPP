// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Unreal_CPP : ModuleRules
{
	public Unreal_CPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
