// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerupActor.generated.h"


class USphereComponent;
class UStaticMeshComponent;


UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASPowerupActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

protected:
	/// <summary>
	/// ����ʱ��
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	/// <summary>
	/// ������ʱ��
	/// </summary>
	FTimerHandle TimerHandle_RespawnTimer;

	/// <summary>
	/// ��ʾ
	/// </summary>
	UFUNCTION()
	void ShowPowerup();

	/// <summary>
	/// ���� & CD
	/// </summary>
	void HideAndCooldownPowerup();

	/// <summary>
	/// ����״̬
	/// </summary>
	/// <param name="bNewIsActive"></param>
	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

public:

	ASPowerupActor();

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) override;

};
