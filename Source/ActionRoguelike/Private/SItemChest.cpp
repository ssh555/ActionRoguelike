// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASItemChest::ASItemChest()
{
	this->BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	this->RootComponent = this->BaseMesh;

	this->LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	this->LidMesh->SetupAttachment(this->BaseMesh);

	this->CanInteractMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CanInteractMesh"));
	this->CanInteractMesh->SetupAttachment(this->BaseMesh);
	this->CanInteractMesh->SetVisibility(false);

	this->TargetPitch = 110.0f;

	/// <summary>
	/// 设置为可复制
	/// UE 会记录此成员，考虑其变量和函数(Server-Client)
	/// </summary>
	this->SetReplicates(true);
	this->bLidOpened = false;
}

// UFUNCTION(Server) -> 客户端 or 服务端 调用 => 服务端执行
void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	// 交互旋转动画
	// Pitch -> Y, Yaw -> Z, Roll -> X
	//LidMesh->SetRelativeRotation(FRotator(this->TargetPitch, 0, 0));
	// 服务端执行 -> 改变时，传递值给客户端，然后执行绑定的 "OnRep_LidOpened"
	// 但服务端不会执行，所以下一行手动调用
	this->bLidOpened = !this->bLidOpened;
	// 服务端执行
	this->OnRep_LidOpened();
}

void ASItemChest::InteractDisplay_Implementation(bool CanInteract, APawn* InstigatorPawn)
{
	this->CanInteractMesh->SetVisibility(CanInteract);
}

void ASItemChest::OnRep_LidOpened_Implementation()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

bool ASItemChest::CanInteract_Implementation(APawn* InstigatorPawn)
{
	return true;
}

// 绑定服务器变量 改变时 需与客户端同步的变量
void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 确保网络中包含此值
	DOREPLIFETIME(ASItemChest, bLidOpened);
}

