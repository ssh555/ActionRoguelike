// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// ����ͷ�ļ������ڴ�֮��
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
	// ȷ����������ĵ�һ��
	GENERATED_BODY()

/// <summary>
/// ����
/// </summary>
protected:
	/* VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention. */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	//UPROPERTY(VisibleAnywhere, Category = "Effects")
	//FName HandSocketName;

	// ������Unity[Label]��������inspector
	/// <summary>
	/// ���ɱ����
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	/// <summary>
	/// ��������
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	///// <summary>
	///// ��������PrimaryAttackObject
	///// </summary>
	//UPROPERTY(EditAnywhere, Category = "Attack") //�ڱ༭���з���
	//TSubclassOf<AActor> ProjectileClass;

	///// <summary>
	///// �ڶ����� Item
	///// </summary>
	//UPROPERTY(EditAnywhere, Category = "Attack")
	//TSubclassOf<AActor> BlackHoleProjectileClass;

	///// <summary>
	///// ��� Item
	///// </summary>
	//UPROPERTY(EditAnywhere, Category = "Attack")
	//TSubclassOf<AActor> DashProjectileClass;

	/// <summary>
	/// �������
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	/// <summary>
	/// �����������
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	/// <summary>
	/// �ж����
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	/////// <summary>
	/////// ��������
	/////// </summary>
	////UPROPERTY(EditAnywhere, Category = "Attack")
	////UAnimMontage* AttackAnim;
	////
	/////* Particle System played during attack animation */
	////UPROPERTY(EditAnywhere, Category = "Attack")
	////UParticleSystem* CastingEffect;
	////
	/////// <summary>
	/////// ��ʱ��Handle
	/////// </summary>
	////FTimerHandle TimerHandle_Attack;
	////
	/////// <summary>
	/////// ���Ŷ��� AttackAnimDelay ���� Item
	/////// </summary>
	////UPROPERTY(EditDefaultsOnly, Category = "Attack")
	////float AttackAnimDelay;




/// <summary>
/// ����
/// </summary>
protected:
	/// <summary>
	/// BindAxis�ƶ�,ǰ���ƶ�
	/// </summary>
	/// <param name="value">�ƶ�����ֵ[-1,1]</param>
	void MoveForward(float value);

	/// <summary>
	/// �����ƶ�
	/// </summary>
	/// <param name="value"></param>
	void MoveRight(float value);

	/// <summary>
	/// ��ʼ���
	/// </summary>
	void SprintStart();

	/// <summary>
	/// �������
	/// </summary>
	void SprintStop();

	/// <summary>
	/// ��Ҫ���� -> ���蹥��
	/// </summary>
	void PrimaryAttack();

	/// <summary>
	/// �ڶ����� -> �ڶ�����
	/// </summary>
	void BlackHoleAttack();

	/// <summary>
	/// ��� -> �������������
	/// </summary>
	void Dash();

	///// <summary>
	///// ���� Item Spawn
	///// </summary>
	//void PrimaryAttack_TimeElapsed();
	///// <summary>
	///// �ڶ����� Item Spawn
	///// </summary>
	//void BlackholeAttack_TimeElapsed();
	///// <summary>
	///// ��� Item Spwan
	///// </summary>
	//void Dash_TimeElapsed();
	///// <summary>
	///// Start ������Ч
	///// </summary>
	//void StartAttackEffects();
	///// <summary>
	///// Re-use spawn logic between attacks
	///// </summary>
	///// <param name="ClassToSpawn"></param>
	//AActor* SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	/// <summary>
	/// ��Ҫ����
	/// </summary>
	void PrimaryInteract();

	/// <summary>
	/// Post Health Change Action
	/// C++ ��ʵ��Ҫ�� _Implementation
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;


/// <summary>
/// ����
/// </summary>
public:
	// Sets default values for this character's properties
	// ���캯��
	ASCharacter();

	// Called to bind functionality to input
	// ��Player����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// Exec -> ����̨����
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
