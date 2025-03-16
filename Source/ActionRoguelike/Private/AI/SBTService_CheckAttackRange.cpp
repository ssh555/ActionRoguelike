// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <AIController.h>

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// check distance(AIPawn, TargetActor)
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp)) {
		AActor* TargetActor =  Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor) {
			AAIController* MyController = OwnerComp.GetAIOwner();
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn)) {
				float DistanceTo = FVector::Distance(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());

				bool bWithinRange = DistanceTo < BlackboardComp->GetValueAsFloat(AttackRangeKey.SelectedKeyName);

				// 检测是否在视线范围内
				bool bHasLOS;
				if (bWithinRange) {
					bHasLOS = MyController->LineOfSightTo(TargetActor);
				}

				BlackboardComp->SetValueAsBool(WithinAttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS);
			}

		}
	}

}
