// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Containers/UnrealString.h>


USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	this->SetIsReplicatedByDefault(true);
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : this->DefaultActions)
	{
		this->AddAction(GetOwner(), ActionClass);
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		this->Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass) const
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	bool ans = false;
	for (USAction* Action : this->Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator)) {
				continue;
			}
			ans = true;
			break;
		}
	}
	if (ans) {
		this->ServerStartAction(Instigator, ActionName, this);
		//if (this->GetOwner()->HasAuthority()) {
		//	MultiCastStartAction_Implementation(Instigator, ActionName, this);
		//}
		//else {
		//	this->ServerStartAction(Instigator, ActionName, this);
		//}
	}

	return ans;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	bool ans = false;
	for (USAction* Action : this->Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->IsRunning()) {
				continue;
			}
			Action->StopAction(Instigator);
			ans = true;
		}
	}

	return ans;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName, USActionComponent* OwnComp)
{
	MultiCastStartAction(Instigator, ActionName, this);
}


void USActionComponent::MultiCastStartAction_Implementation(AActor* Instigator, FName ActionName, USActionComponent* OwnComp)
{
	for (USAction* Action : OwnComp->Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator)) {
				continue;
			}
			Action->StartAction(Instigator);
		}
	}
}


