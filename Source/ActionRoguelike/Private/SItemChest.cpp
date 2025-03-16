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
	/// ����Ϊ�ɸ���
	/// UE ���¼�˳�Ա������������ͺ���(Server-Client)
	/// </summary>
	this->SetReplicates(true);
	this->bLidOpened = false;
}

// UFUNCTION(Server) -> �ͻ��� or ����� ���� => �����ִ��
void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	// ������ת����
	// Pitch -> Y, Yaw -> Z, Roll -> X
	//LidMesh->SetRelativeRotation(FRotator(this->TargetPitch, 0, 0));
	// �����ִ�� -> �ı�ʱ������ֵ���ͻ��ˣ�Ȼ��ִ�а󶨵� "OnRep_LidOpened"
	// ������˲���ִ�У�������һ���ֶ�����
	this->bLidOpened = !this->bLidOpened;
	// �����ִ��
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

// �󶨷��������� �ı�ʱ ����ͻ���ͬ���ı���
void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ȷ�������а�����ֵ
	DOREPLIFETIME(ASItemChest, bLidOpened);
}

