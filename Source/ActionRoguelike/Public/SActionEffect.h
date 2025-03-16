// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

protected:
	/// <summary>
	/// Effect ����ʱ�䣬��Ϊ 0 �����ô���
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	/// <summary>
	/// ѭ��ִ��,��Ϊ 0 ��ѭ��ִ��
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	//bool bIsAutoExe = true;

	FTimerHandle PeriodHandle;

	FTimerHandle DurationHandle;



	/// <summary>
	/// ����ִ��Ч��
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecuteEffect(AActor* Instigator);

	/// <summary>
	/// ����ѭ��ִ��Ч��
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

public:
	USActionEffect();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;
	
};
