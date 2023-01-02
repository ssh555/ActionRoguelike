// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

// ���ɱ����include
#include "GameFramework/SpringArmComponent.h"
// ��������include
#include "Camera/CameraComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->PrimaryActorTick.bCanEverTick = true;

	// ��������
	this->SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	// ���帽�ţ�child->parent
	// RootComponent ������ĸ����,����νṹ�еĵ�һ�����
	this->SpringArmComp->SetupAttachment(this->RootComponent);
	
	this->CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	this->CameraComp->SetupAttachment(this->SpringArmComp);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float value)
{
	// ������Pawn
	// ����Scaleֵ, �Ƿ�����������
	this->AddMovementInput(this->GetActorForwardVector(), value);
}

void ASCharacter::MoveRight(float value)
{
	// ������Pawn
// ����Scaleֵ, �Ƿ�����������
	this->AddMovementInput(this->GetActorRightVector(), value);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ���ᣬ������unity�е�inputmanager
	// AxisString, UserObject, InvokeFunction
	// Edit -> Project Settings -> Input -> Bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}

