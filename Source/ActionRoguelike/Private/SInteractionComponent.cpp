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

	// 均有N个PlayerCharacter，则有N个此组件，因此都会执行
	// 加上if 本地只有自己控制的这个会执行
	// 不加if 若此函数内没有任何表现层的代码，则渲染出来的画面没有影响，但依旧执行
	// 因为此检测只运行在本地，对其他端没有作用，因此加上if
	if (this->MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}

}

void USInteractionComponent::FindBestInteractable()
{
	/// region 交互对象碰撞检测
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

	// 设置 交互对象
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

	// 显示可以交互信息
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
	/// 传参数 => Find只运行本地Player,所以本地的FocusedActor指向正确
	/// 但是运行服务端的此函数，则服务端找到与本地对应的处于服务端的那个Player执行此函数
	/// 但是服务器的此Player的Find没有执行，因此服务器的Focus为null
	/// 因此传参数可以保证双端的Focus同步
	/// 至于服务端如何知道服务端的哪个Object与本地的Focus对应，交给UE
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

