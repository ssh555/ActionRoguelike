// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include <DrawDebugHelpers.h>
#include "SWorldUserWidget.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	this->PrimaryComponentTick.bCanEverTick = true;

	this->HitActors = new TArray<AActor*>();

	this->TraceRadius = 30.0f;
	this->TraceDistance = 500.0f;
	this->CollisionChannel = ECC_WorldDynamic;

}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	this->MyPawn = Cast<APawn>(GetOwner());
}

// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ����N��PlayerCharacter������N�����������˶���ִ��
	// ����if ����ֻ���Լ����Ƶ������ִ��
	// ����if ���˺�����û���κα��ֲ�Ĵ��룬����Ⱦ�����Ļ���û��Ӱ�죬������ִ��
	// ��Ϊ�˼��ֻ�����ڱ��أ���������û�����ã���˼���if
	if (this->MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}

}

void USInteractionComponent::FindBestInteractable()
{
	/// region ����������ײ���
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	/// endregion

	// ���� ��������
	// Clear ref before trying to fill
	if (this->FocusedActor) {
		ISGameplayInterface::Execute_InteractDisplay(this->FocusedActor, false, Cast<APawn>(this->GetOwner()));
	}
	this->FocusedActor = nullptr;

	//ISGameplayInterface::Execute_CanInteractDisplay(HitActor, false, Cast<APawn>(this->GetOwner()));

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				
				//if (Cast<ISGameplayInterface>(HitActor)->CanInteract(Cast<APawn>(HitActor))) {
				if (ISGameplayInterface::Execute_CanInteract(HitActor, Cast<APawn>(this->GetOwner()))) {
					this->FocusedActor = HitActor;
					break;
				}
			}
		}
	}

	// ��ʾ���Խ�����Ϣ
	if (this->FocusedActor)
	{
		ISGameplayInterface::Execute_InteractDisplay(this->FocusedActor, true, Cast<APawn>(this->GetOwner()));
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

}

void USInteractionComponent::PrimaryInteract()
{
	/// <summary>
	/// ������ => Findֻ���б���Player,���Ա��ص�FocusedActorָ����ȷ
	/// �������з���˵Ĵ˺������������ҵ��뱾�ض�Ӧ�Ĵ��ڷ���˵��Ǹ�Playerִ�д˺���
	/// ���Ƿ������Ĵ�Player��Findû��ִ�У���˷�������FocusΪnull
	/// ��˴��������Ա�֤˫�˵�Focusͬ��
	/// ���ڷ�������֪������˵��ĸ�Object�뱾�ص�Focus��Ӧ������UE
	/// </summary>
	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	ISGameplayInterface::Execute_Interact(InFocus, this->MyPawn);
}

