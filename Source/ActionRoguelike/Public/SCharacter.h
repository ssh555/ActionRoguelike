// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	// 确保保持在类的第一行
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	// 构造函数
	ASCharacter();

protected:
	// 弹簧臂组件
	// 类似于unity[LABEL]应用于inspector面板
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	// 摄像机组件
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	// Called when the game starts or when spawned
	// 生成时调用一次，即只要生成就调用一次，也只会调用一次
	virtual void BeginPlay() override;

	/// <summary>
	/// BindAxis移动
	/// </summary>
	/// <param name="value">移动参数值[-1,1]</param>
	void MoveForward(float value);

	void MoveRight(float value);

public:	
	// Called every frame
	// 每帧调用
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// 绑定Player输入
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
