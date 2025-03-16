// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Struct/NPBPFunctionLibrary.h>
#include <NetPacketProcessor.h>
#include "SGameplayFunctionLibrary.generated.h"

struct Ftemplate_ue;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/// <summary>
	/// ֱ��Ӧ���˺�
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);
	
	/// <summary>
	/// & -> ��ͼ���
	/// const & -> ��ͼ����
	/// Ӧ���˺�����ɳ��
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static void TestNP(const Ftemplate_ue& t, Ftemplate_ue& r);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static void TestNPDelegate(UStruct* stype, FRegisterProcessDelegate Delegate);

	static NetPacket::NetPacketProcessor processor;

};
