// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SProjectileBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/GameSession.h>
#include <GameFramework/PlayerState.h>
#include <SCharacter.h>
#include <Camera/CameraComponent.h>




USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	// 发射点Socket
	this->HandSocketName = "Muzzle_01";
	this->AttackAnimDelay = 0.2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	// 之前的没有结束, 即上一次的Spawn之后才能进行下一次攻击
	if (this->TimerHandle_AttackDelay.IsValid())
		return;

	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(this->AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(this->CastingEffect, Character->GetMesh(), this->HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(this->TimerHandle_AttackDelay, Delegate, this->AttackAnimDelay, false);

		// 转向攻击方向
		if (APawn* myPawn = Cast<APawn>(Instigator)) {
			FRotator rot = myPawn->GetControlRotation();
			rot.Pitch = rot.Roll = 0;
			myPawn->SetActorRotation(rot);
		}

	}
}

ASProjectileBase* USAction_ProjectileAttack::SpawnProjectile(ACharacter* InstigatorCharacter)
{
	ASProjectileBase* ans = nullptr;
	if (ensureAlways(this->ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		ObjParams.AddObjectTypesToQuery(ECC_Vehicle);
		ObjParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

		// endpoint far into the look-at distance (not too far, still adjust somewhat towards cross hair on a miss)
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 10000);
		if (ASCharacter* SCh = Cast<ASCharacter>(InstigatorCharacter)) {
			TraceEnd = TraceStart + (SCh->ControlRotation.Vector() * 10000);
		}

		FHitResult Hit;
		// returns true if we got to a blocking hit
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			// Overwrite trace end with impact point in world
			TraceEnd = Hit.ImpactPoint;
		}

		// find new direction/rotation from Hand pointing to impact point in world.
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		ans = (GetWorld()->SpawnActor<ASProjectileBase>(ProjectileClass, SpawnTM, SpawnParams));
	}
	return ans;
}

void USAction_ProjectileAttack::AttackDelay_Elapsed_Implementation(ACharacter* InstigatorCharacter)
{
	// Clear TimeHandle
	this->TimerHandle_AttackDelay.Invalidate();

	SpawnProjectile(InstigatorCharacter);

	StopAction(InstigatorCharacter);
}
