// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include <Perception/PawnSensingComponent.h>
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <DrawDebugHelpers.h>
#include "SAttributeComponent.h"
#include <BrainComponent.h>
#include <GameFramework/PlayerController.h>
#include "SWorldUserWidget.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "SActionComponent.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ASAICharacter::ASAICharacter()
{
	this->PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	// Ensures we receive a controlled when spawned in the level by our game-mode
	// BPSelf -> Pawn -> Auto Possess AI -> Changed
	this->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	this->AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	this->ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	// Disabled on capsule to let projectiles pass through capsule and hit mesh instead
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	// Enabled on mesh to react to incoming projectiles
	GetMesh()->SetGenerateOverlapEvents(true);

	this->TimeToHitParamName = "TimeToHit";
	TargetActorKey = "TargetActor";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	this->PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	// Ignore if target already set
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);

		USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (NewWidget)
		{
			NewWidget->AttachedActor = this;
			// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
			// May end up behind the minion health bar otherwise.
			NewWidget->AddToViewport(10);
		}
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// 受伤
	if (Delta < 0.0f) {
		if (InstigatorActor != this) {
			this->SetTargetActor(InstigatorActor); 
		}

		// 血条未激活	
		if (this->ActiveHealthBar == nullptr) {
			this->ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), this->HealthBarWidgetClass);
			if (this->ActiveHealthBar) {
				this->ActiveHealthBar->AttachedActor = this;
				if (USAttributeComponent::IsActorAlive(this)) {
					this->ActiveHealthBar->AddToViewport();
				}
			}
		}


		this->GetMesh()->SetScalarParameterValueOnMaterials(this->TimeToHitParamName, GetWorld()->TimeSeconds);

		// 死亡
		if (NewHealth <= 0.0f) {
			// Stop BT
			AAIController* AIC = Cast<AAIController>(this->GetController());
			if (AIC) {
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			// Ragdoll
			// 启用物理学模拟
			this->GetMesh()->SetAllBodiesSimulatePhysics(true);
			// 设置碰撞为Ragdoll,UE自带
			this->GetMesh()->SetCollisionProfileName("Ragdoll");
			// 禁用碰撞
			this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//禁用移动
			this->GetCharacterMovement()->DisableMovement();

			// Set Lifespan
			// 还能存在多久
			// InitialLifeSpan -> Spawn后能存在多久
			this->SetLifeSpan(10.0f);
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(this->GetController());
	if (AIC) {
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject(this->TargetActorKey, NewTarget);

		//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}

AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}



