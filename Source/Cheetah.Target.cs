// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class CheetahTarget : TargetRules
{
	public CheetahTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        CppStandard = CppStandardVersion.Cpp20;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;

        ExtraModuleNames.AddRange( new string[] { "Cheetah" } );
	}
}
