// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 编写接口
 */
class ACTIONROGUELIKE_API ISGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	/// <summary>
	/// 当处于可交互状态时是否能交互 (即除了交互碰撞检测外还有其他限制
	/// 需要在此接口实现一下，否则报错：无法解析的外部符号
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanInteract(APawn* InstigatorPawn);

	/// <summary>
	/// 交互现象显示
	/// </summary>
	/// <param name="CanInteract">是否能交互及其对应显示状态</param>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractDisplay(bool CanInteract, APawn* InstigatorPawn);

	/// <summary>
	/// 交互接口函数
	/// </summary>
	/// <param name="InstigatorPawn">交互发起者</param>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(APawn* InstigatorPawn);

};
