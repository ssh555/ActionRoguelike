// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/KismetSystemLibrary.h>




int32 ASPlayerState::GetCredits()
{
	return this->Credits;
}

void ASPlayerState::AddCredits(int32 Delta)
{
	// Avoid user-error of adding a negative amount or zero
	if (!ensure(Delta > 0.0f))
	{
		return;
	}

	Credits += Delta;
	MultiCastCreditsChanged(this, Credits, Delta);
}


bool ASPlayerState::RemoveCredits(int32 Delta)
{
	// Avoid user-error of adding a subtracting negative amount or zero
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (Credits < Delta)
	{
		// Not enough credits available
		return false;
	}

	Credits -= Delta;

	MultiCastCreditsChanged(this, Credits, -Delta);

	return true;
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax, COND_InitialOnly);
}

void ASPlayerState::MultiCastCreditsChanged_Implementation(ASPlayerState* PlayerState, int32 NewCredits, int32 Delta)
{
	OnCreditsChanged.Broadcast(PlayerState, NewCredits, Delta);
}

