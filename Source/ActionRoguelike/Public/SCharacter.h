// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// 所有头文件必须在此之上
#include "SCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;
class ASMagicProjectile;
class USAttributeComponent;
class UParticleSystem;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	// 确保保持在类的第一行
	GENERATED_BODY()

/// <summary>
/// 变量
/// </summary>
protected:
	/* VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention. */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	//UPROPERTY(VisibleAnywhere, Category = "Effects")
	//FName HandSocketName;

	// 类似于Unity[Label]，作用于inspector
	/// <summary>
	/// 弹簧臂组件
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	/// <summary>
	/// 摄像机组件
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	///// <summary>
	///// 用于生成PrimaryAttackObject
	///// </summary>
	//UPROPERTY(EditAnywhere, Category = "Attack") //在编辑器中分类
	//TSubclassOf<AActor> ProjectileClass;

	///// <summary>
	///// 黑洞攻击 Item
	///// </summary>
	//UPROPERTY(EditAnywhere, Category = "Attack")
	//TSubclassOf<AActor> BlackHoleProjectileClass;

	///// <summary>
	///// 冲刺 Item
	///// </summary>
	//UPROPERTY(EditAnywhere, Category = "Attack")
	//TSubclassOf<AActor> DashProjectileClass;

	/// <summary>
	/// 交互组件
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	/// <summary>
	/// 属性数据组件
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	/// <summary>
	/// 行动组件
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	/////// <summary>
	/////// 攻击动画
	/////// </summary>
	////UPROPERTY(EditAnywhere, Category = "Attack")
	////UAnimMontage* AttackAnim;
	////
	/////* Particle System played during attack animation */
	////UPROPERTY(EditAnywhere, Category = "Attack")
	////UParticleSystem* CastingEffect;
	////
	/////// <summary>
	/////// 定时器Handle
	/////// </summary>
	////FTimerHandle TimerHandle_Attack;
	////
	/////// <summary>
	/////// 播放动画 AttackAnimDelay 生成 Item
	/////// </summary>
	////UPROPERTY(EditDefaultsOnly, Category = "Attack")
	////float AttackAnimDelay;




/// <summary>
/// 函数
/// </summary>
protected:
	/// <summary>
	/// BindAxis移动,前后移动
	/// </summary>
	/// <param name="value">移动参数值[-1,1]</param>
	void MoveForward(float value);

	/// <summary>
	/// 左右移动
	/// </summary>
	/// <param name="value"></param>
	void MoveRight(float value);

	/// <summary>
	/// 开始冲刺
	/// </summary>
	void SprintStart();

	/// <summary>
	/// 结束冲刺
	/// </summary>
	void SprintStop();

	/// <summary>
	/// 主要攻击 -> 弹丸攻击
	/// </summary>
	void PrimaryAttack();

	/// <summary>
	/// 黑洞攻击 -> 黑洞拉扯
	/// </summary>
	void BlackHoleAttack();

	/// <summary>
	/// 冲刺 -> 闪现至攻击落点
	/// </summary>
	void Dash();

	///// <summary>
	///// 攻击 Item Spawn
	///// </summary>
	//void PrimaryAttack_TimeElapsed();
	///// <summary>
	///// 黑洞攻击 Item Spawn
	///// </summary>
	//void BlackholeAttack_TimeElapsed();
	///// <summary>
	///// 冲刺 Item Spwan
	///// </summary>
	//void Dash_TimeElapsed();
	///// <summary>
	///// Start 攻击特效
	///// </summary>
	//void StartAttackEffects();
	///// <summary>
	///// Re-use spawn logic between attacks
	///// </summary>
	///// <param name="ClassToSpawn"></param>
	//AActor* SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	/// <summary>
	/// 主要交互
	/// </summary>
	void PrimaryInteract();

	/// <summary>
	/// Post Health Change Action
	/// C++ 内实现要加 _Implementation
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;


/// <summary>
/// 函数
/// </summary>
public:
	// Sets default values for this character's properties
	// 构造函数
	ASCharacter();

	// Called to bind functionality to input
	// 绑定Player输入
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// Exec -> 控制台函数
	/// </summary>
	UFUNCTION(Exec)
	void PlayerHeal(float Amount = 100);

	UFUNCTION(Exec)
	void PlayerSuicide();

	UCameraComponent* GetCameraComp();

	UPROPERTY(Replicated)
	FRotator ControlRotation;

	UFUNCTION(Server, Reliable)
	void ServerSetControlRotation(FRotator rot);

	virtual void AddControllerPitchInput(float Val) override;

	virtual void AddControllerYawInput(float Val) override;

	virtual void AddControllerRollInput(float Val) override;

	UPROPERTY(EditAnywhere)
	TArray<FGuid> guid;

	UFUNCTION(BlueprintCallable)
	static FGuid NewGuid();
};
