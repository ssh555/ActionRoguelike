// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class APlayerController;

/**
 * window ->world settings -> GameMode -> GameMode Override ��Ϊ None ʹ�� Default Mode
 * ÿ�� Level �� GameMode ����
 * BPSelf -> Classes -> Defualt Pawn Class ���� [Player Start] ʹ��
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// BPSelf -> Pawn -> Auto Possess AI -> Changed
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	/// <summary>
	/// �������� SpawnBot
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	/// <summary>
	/// SpwanBots_Timer
	/// </summary>
	FTimerHandle TimerHandle_SpawnBots;
	
	/// <summary>
	/// SpawnBotsInterval
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	// Read/write access as we could change this as our difficulty increases via Blueprint
	/// <summary>
	/// ɱ��һ��Enemy�ɻ�õ�Credits
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;

	/// <summary>
	/// Powerup������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	UEnvQuery* PowerupSpawnQuery;

	/* All power-up classes used to spawn with EQS at match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	/* Distance required between power-up spawn locations */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float RequiredPowerupDistance;

	/* Amount of powerups to spawn during match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 DesiredPowerupCount;

	/// <summary>
	/// ��ʱ��������
	/// </summary>
	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:
	ASGameModeBase();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="VictimActor">�ܺ���</param>
	/// <param name="Killer">ɱ��</param>
	UFUNCTION()
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	/** Transitions to calls BeginPlay on actors. */
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAllEnemy();
	
};
