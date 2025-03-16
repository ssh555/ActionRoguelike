// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
	if (!ensure(WorldContext))
	{
		return;
	}
	// 获取 object 所属 World
	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}
	// 处于 Client or Server
	FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
	/** Global engine pointer. Can be 0 so don't use without checking. */
	if (GEngine)
	{
		/// <summary>
		/// 屏幕上PrintMsg
		/// </summary>
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}