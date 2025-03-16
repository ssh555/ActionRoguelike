// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	/// <summary>
	/// 传送延迟
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	/// <summary>
	/// 爆炸延迟 -> 生命周期
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	// Handle to cancel timer if we already hit something
	FTimerHandle TimerHandle_DelayedDetonate;

	// Base class using BlueprintNativeEvent, we must override the _Implementation not the Explode()
	virtual void Explode_Implementation() override;

	/// <summary>
	/// 传送 Instigator
	/// </summary>
	void TeleportInstigator();

	virtual void BeginPlay() override;

public:

	ASDashProjectile();
};
