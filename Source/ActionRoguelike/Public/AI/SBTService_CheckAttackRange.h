// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckAttackRange.generated.h"

/**
 * ������Ϊ���ڵ� AddService
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	/// <summary>
	/// ������ͼ�������˵�
	/// ��Ϊ�� Service ->Details
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector WithinAttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;

	/** update next tick interval
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
