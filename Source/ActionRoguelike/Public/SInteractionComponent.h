// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	APawn* MyPawn;
	/// <summary>
	/// 射线检测对象
	/// </summary>
	TArray<AActor*> *HitActors;

	/// <summary>
	/// 交互对象
	/// </summary>
	UPROPERTY()
	AActor* FocusedActor;

	/// <summary>
	/// 交互检测距离
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	/// <summary>
	/// 交互检测半径
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	/// <summary>
	/// 碰撞检测通道
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	/// <summary>
	/// 提示UIClass
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	/// <summary>
	/// 提示UIInstance
	/// </summary>
	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;

public:	
	// Reliable - Will always arrive, eventually. Request will be re-sent unless an acknowledgment was received.
	// Unreliable - Not guaranteed, packet can get lost and won't retry.
	// 本地 或者 服务器 调用，均在服务器端执行
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	/// <summary>
	/// 交互发起者调用的交互函数
	/// </summary>
	void PrimaryInteract();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindBestInteractable();

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
