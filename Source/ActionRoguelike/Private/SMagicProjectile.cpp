// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "SActionComponent.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Net/UnrealNetwork.h"


ASMagicProjectile::ASMagicProjectile()
{
	this->SphereComp->SetSphereRadius(20.0f);
	this->SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);

	this->DamageAmount = 20.0f;

}



void ASMagicProjectile::SetDamageAmount(float NewDMA)
{
	if(!this->IsPendingKill())
		this->DamageAmount = FMath::Max(NewDMA, 0.0f);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		// ·´µ¯
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(this->ParryTag)) {
			this->MoveComp->Velocity = -this->MoveComp->Velocity;

			this->SetInstigator(Cast<APawn>(OtherActor));

			return;
		}

		//USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		//if (AttributeComp)
		//{
		//	// minus in front of DamageAmount to apply the change as damage, not healing
		//	AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);

		//	// Only explode when we hit something valid
		//	Explode();
		//}
		if (USGameplayFunctionLibrary::ApplyDamage(GetInstigator(), OtherActor, DamageAmount)) {
			Explode();
		}
		//USGameplayFunctionLibrary::ApplyDamage(GetInstigator(), OtherActor, DamageAmount);
	}
}

void ASMagicProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASMagicProjectile, DamageAmount);
	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax, COND_InitialOnly);
}