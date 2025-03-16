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
 * ��д�ӿ�
 */
class ACTIONROGUELIKE_API ISGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	/// <summary>
	/// �����ڿɽ���״̬ʱ�Ƿ��ܽ��� (�����˽�����ײ����⻹����������
	/// ��Ҫ�ڴ˽ӿ�ʵ��һ�£����򱨴��޷��������ⲿ����
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanInteract(APawn* InstigatorPawn);

	/// <summary>
	/// ����������ʾ
	/// </summary>
	/// <param name="CanInteract">�Ƿ��ܽ��������Ӧ��ʾ״̬</param>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractDisplay(bool CanInteract, APawn* InstigatorPawn);

	/// <summary>
	/// �����ӿں���
	/// </summary>
	/// <param name="InstigatorPawn">����������</param>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(APawn* InstigatorPawn);

};
