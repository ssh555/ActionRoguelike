// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include <BehaviorTree/BehaviorTree.h>
#include <Kismet/GameplayStatics.h>
#include <BehaviorTree/BlackboardComponent.h>

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(this->BehaviorTree, TEXT("BehaviorTree is nullptr! Please assign BehaviorTree in your AI Controller."))) {
		// ������Ϊ��
		RunBehaviorTree(BehaviorTree);
	}


	// ��ȡ index = 0 �� player
	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (MyPawn) {
		//// Or this->Blackboard
		//this->GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

		//this->GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);

		this->GetBlackboardComponent()->SetValueAsFloat("AttackRange", this->AttackRange);
	}
}
