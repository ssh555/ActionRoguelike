// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/// <summary>
/// Credits ChangedT Trigger
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);


/**
 * GameMode -> Classes -> Player State Class
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Credits")
	int32 Credits;

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastCreditsChanged(ASPlayerState* PlayerState, int32 NewCredits, int32 Delta);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure ,Category = "Credits")
	int32 GetCredits();

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;
};
