// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	USActionComponent* Comp = this->GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(this->GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(this->bIsRunning);

	USActionComponent* Comp = this->GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(this->GrantsTags);

	this->bIsRunning = false;
}

bool USAction::IsRunning() const
{
	return this->bIsRunning;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (this->IsRunning())
	{
		return false;
	}

	USActionComponent* Comp = this->GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(this->BlockedTags)) {
		return false;
	}
	return true;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(this->GetOuter());
}

UWorld* USAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(this->GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}
