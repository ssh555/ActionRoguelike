// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionRoguelike : ModuleRules
{
	public ActionRoguelike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayTasks", "AIModule", "UMG", "GameplayTags", "NetPacket" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// 添加插件的公共和私有包含路径
		PublicIncludePaths.AddRange(
			new string[] {
				"../Plugins/NetPacket/Source/NetPacket/Public"
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				"../Plugins/NetPacket/Source/NetPacket/Private"
			}
		);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
