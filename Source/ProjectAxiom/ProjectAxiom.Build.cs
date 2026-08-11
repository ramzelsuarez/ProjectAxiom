// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ProjectAxiom : ModuleRules
{
	public ProjectAxiom(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"GameplayTags",
			"PhysicsCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput" });
		
	}
}
