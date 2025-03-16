// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_HealthPotion.generated.h"


class UStaticMeshComponent;


/**
 * �ظ��� HP
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerup_HealthPotion : public ASPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthPotion")
	int32 CreditCost;

	// float health amount?

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	ASPowerup_HealthPotion();
};
