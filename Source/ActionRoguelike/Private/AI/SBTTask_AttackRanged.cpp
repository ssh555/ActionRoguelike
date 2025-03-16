// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_AttackRanged.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <AIController.h>
#include <GameFramework/Character.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Engine/World.h>
#include "SAttributeComponent.h"
#include "SMagicProjectile.h"

USBTTask_AttackRanged::USBTTask_AttackRanged()
{
	this->MaxBulletSpread = 2.0f;
	this->DamageAmount = 5.0f;
}

EBTNodeResult::Type USBTTask_AttackRanged::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// BT的AIController
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController)) {
		// 所属APawn
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr) {
			return EBTNodeResult::Failed;
		}

		// 攻击发射点
		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr) {
			return EBTNodeResult::Failed;
		}

		// 目标已死亡
		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		// Muzzle -> TargetActor
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
		if (NewProj) {
			ASMagicProjectile* Proj = Cast<ASMagicProjectile>(NewProj);
			if (Proj) {
				Proj->SetDamageAmount(this->DamageAmount);
			}
		}
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;

}
