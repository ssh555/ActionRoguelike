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
	/// Effect 持续时间，若为 0 则永久存在
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	/// <summary>
	/// 循环执行,若为 0 则不循环执行
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	//bool bIsAutoExe = true;

	FTimerHandle PeriodHandle;

	FTimerHandle DurationHandle;



	/// <summary>
	/// 立即执行效果
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecuteEffect(AActor* Instigator);

	/// <summary>
	/// 周期循环执行效果
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

public:
	USActionEffect();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;
	
};
