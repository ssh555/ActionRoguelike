// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;
class ASProjectileBase;

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:
	/// <summary>
	/// 用于生成PrimaryAttackObject
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effects")
	FName HandSocketName;

	/// <summary>
	/// 播放动画 AttackAnimDelay 生成 Item
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	/// <summary>
	/// 攻击动画
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerHandle_AttackDelay;

	UFUNCTION(BlueprintCallable)
	ASProjectileBase* SpawnProjectile(ACharacter* InstigatorCharacter);

	UFUNCTION(BlueprintNativeEvent)
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);


public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	USAction_ProjectileAttack();
};
