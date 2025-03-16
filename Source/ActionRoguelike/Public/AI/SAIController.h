// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	/// <summary>
	/// ʹ�õ���Ϊ��asset
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
