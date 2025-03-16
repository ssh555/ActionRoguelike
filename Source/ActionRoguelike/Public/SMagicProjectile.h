// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include <GameplayTagContainer.h>
#include "SMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBase // Re-parented from AActor
{
	GENERATED_BODY()

protected:
	/// <summary>
	/// √¸÷–…À∫¶÷µ
	/// </summary>
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTag ParryTag;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	ASMagicProjectile();

	UFUNCTION(BlueprintCallable)
	void SetDamageAmount(float NewDMA);

};
