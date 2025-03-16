// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"
#include <SActionComponent.h>



USActionEffect::USActionEffect()
{
	this->bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (this->Duration > 0.0f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(this->DurationHandle, Delegate, this->Duration, false);
	}

	this->ExecuteEffect(Instigator);

	if (this->Period > 0.0f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(this->PeriodHandle, Delegate, this->Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(this->PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(this->DurationHandle);

	USActionComponent* Comp = this->GetOwningComponent();
	if (Comp) {
		Comp->RemoveAction(this);
	}
}

void USActionEffect::ExecuteEffect_Implementation(AActor* Instigator)
{

}



void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}