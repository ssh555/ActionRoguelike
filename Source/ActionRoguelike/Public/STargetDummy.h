// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"


class USAttributeComponent;


UCLASS()
class ACTIONROGUELIKE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()

public:

	ASTargetDummy();

protected:

	/// <summary>
	/// 属性组件
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	USAttributeComponent* AttributeComp;

	/// <summary>
	/// 静态网格组件
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	/// <summary>
	/// Post Health Change Action
	/// </summary>
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
