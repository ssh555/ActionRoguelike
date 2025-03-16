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
	/// ���߼�����
	/// </summary>
	TArray<AActor*> *HitActors;

	/// <summary>
	/// ��������
	/// </summary>
	UPROPERTY()
	AActor* FocusedActor;

	/// <summary>
	/// ����������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	/// <summary>
	/// �������뾶
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	/// <summary>
	/// ��ײ���ͨ��
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	/// <summary>
	/// ��ʾUIClass
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	/// <summary>
	/// ��ʾUIInstance
	/// </summary>
	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;

public:	
	// Reliable - Will always arrive, eventually. Request will be re-sent unless an acknowledgment was received.
	// Unreliable - Not guaranteed, packet can get lost and won't retry.
	// ���� ���� ������ ���ã����ڷ�������ִ��
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	/// <summary>
	/// ���������ߵ��õĽ�������
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
