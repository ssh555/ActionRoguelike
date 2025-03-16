// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

// 静态网格组件
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()


public:
	/// <summary>
	/// LidMesh Pitch 旋转角
	/// </summary>
	UPROPERTY(EditAnywhere)
	float TargetPitch;




protected:
	/// <summary>
	/// ReplicatedUsing 变量在服务器改变时 调用 "OnRep_LidOpened"
	/// 需要在 GetLifetimeReplicatedProps 中绑定
	/// 只传递给 客户端 触发，服务端需要手动调用 -> 因为有些调用只是表现层，而服务端可以只运行逻辑层，所以不会自动调用
	/// </summary>
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadOnly) // RepNotify
	bool bLidOpened;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnRep_LidOpened();

	// 底座
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	// 盖子
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	// 可交互时显示的Mesh
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CanInteractMesh;

public:	
	// Sets default values for this actor's properties
	ASItemChest();

	/// <summary>
	/// ISGameplayInterface 交互函数接口
	/// </summary>
	/// <param name="InstigatorPawn">交互发起者</param>
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	/// <summary>
	/// 交互现象显示
	/// </summary>
	/// <param name="CanInteract">是否能交互</param>
	virtual void InteractDisplay_Implementation(bool CanInteract, APawn* InstigatorPawn) override;

	UFUNCTION()
	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
