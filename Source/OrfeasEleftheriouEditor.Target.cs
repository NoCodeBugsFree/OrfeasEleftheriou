// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class OrfeasEleftheriouEditorTarget : TargetRules
{
	public OrfeasEleftheriouEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "OrfeasEleftheriou" } );
	}
}
