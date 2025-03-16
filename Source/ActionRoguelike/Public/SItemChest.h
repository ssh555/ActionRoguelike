// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

// ��̬�������
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()


public:
	/// <summary>
	/// LidMesh Pitch ��ת��
	/// </summary>
	UPROPERTY(EditAnywhere)
	float TargetPitch;




protected:
	/// <summary>
	/// ReplicatedUsing �����ڷ������ı�ʱ ���� "OnRep_LidOpened"
	/// ��Ҫ�� GetLifetimeReplicatedProps �а�
	/// ֻ���ݸ� �ͻ��� �������������Ҫ�ֶ����� -> ��Ϊ��Щ����ֻ�Ǳ��ֲ㣬������˿���ֻ�����߼��㣬���Բ����Զ�����
	/// </summary>
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadOnly) // RepNotify
	bool bLidOpened;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnRep_LidOpened();

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	// �ɽ���ʱ��ʾ��Mesh
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CanInteractMesh;

public:	
	// Sets default values for this actor's properties
	ASItemChest();

	/// <summary>
	/// ISGameplayInterface ���������ӿ�
	/// </summary>
	/// <param name="InstigatorPawn">����������</param>
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	/// <summary>
	/// ����������ʾ
	/// </summary>
	/// <param name="CanInteract">�Ƿ��ܽ���</param>
	virtual void InteractDisplay_Implementation(bool CanInteract, APawn* InstigatorPawn) override;

	UFUNCTION()
	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
