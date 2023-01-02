// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

// 弹簧臂组件include
#include "GameFramework/SpringArmComponent.h"
// 摄像机组件include
#include "Camera/CameraComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->PrimaryActorTick.bCanEverTick = true;

	// 创建物体
	this->SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	// 物体附着，child->parent
	// RootComponent 此物体的根组件,即层次结构中的第一个组件
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
	// 来自于Pawn
	// 方向，Scale值, 是否启用力作用
	this->AddMovementInput(this->GetActorForwardVector(), value);
}

void ASCharacter::MoveRight(float value)
{
	// 来自于Pawn
// 方向，Scale值, 是否启用力作用
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

	// 绑定轴，类似于unity中的inputmanager
	// AxisString, UserObject, InvokeFunction
	// Edit -> Project Settings -> Input -> Bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}

