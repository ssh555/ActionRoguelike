// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "SInteractionComponent.h"
#include <DrawDebugHelpers.h>
#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include <Components/CapsuleComponent.h>
#include "Kismet/GameplayStatics.h"
#include <Engine/EngineTypes.h>
#include "SGameplayFunctionLibrary.h"
#include "SActionComponent.h"
#include "Net/UnrealNetwork.h"

/*
	1. 纯C++类型 => new
	2. UObject(U 开头 => NewObject
	3. Actor(A 开头 => World->SpawnActor
	4. Component => CreateDefaultSubobject
	5. UE资源 => StaticLoadObject
	6. 其他文件资源 => 正常 Load
*/

// Sets default values
ASCharacter::ASCharacter()
{
	// 设置是否启用 Tick
	this->PrimaryActorTick.bCanEverTick = false;

	// 创建物体
	// 进入文件实际存在位置 Classes/[include]
	this->SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");

	this->ControlRotation = this->GetControlRotation();
	// 物体附着，child->parent
	// RootComponent 此物体的根组件,即层次结构中的第一个组件
	this->SpringArmComp->SetupAttachment(this->RootComponent);
	// 见 ASCharacter::SetupPlayerInputComponent
	SpringArmComp->bUsePawnControlRotation = true;

	this->CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	this->CameraComp->SetupAttachment(this->SpringArmComp);

	// 见 ASCharacter::SetupPlayerInputComponent
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationRoll = false;
	this->GetCharacterMovement()->bOrientRotationToMovement = true;

	//// 设置PrimaryAttackObject的SpawnClass
	//// LoadClass<T>
	//// Or 在面板设置 PlayerCharacter -> SCharacter -> ProjectileClass
	//this->ProjectileClass = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/ActionRoguelike/MagicProjectileBP.MagicProjectileBP_C'"));


	this->InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	this->AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	this->ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	this->TimeToHitParamName = "TimeToHit";


	//this->AttackAnimDelay = 0.2f;
	//this->HandSocketName = "Muzzle_01";
	//this->ProjectileDamageAmount = 20.0f;
	
	UE_LOG(LogTemp, Warning, TEXT("Temp Class: %s"), *GetClass()->GetName());
	for (int i = 0; i < 5; ++i)
	{
		guid.Add(FGuid::NewGuid());
		UE_LOG(LogTemp, Warning, TEXT("Temp GUID: %s"), *guid[i].ToString());
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	this->AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

/*
	右手系
	X -> Forward
	Y -> Right
	Z -> Up
*/

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = this->GetControlRotation();
	// 来自于Pawn
	// 方向，Scale值, 是否启用力作用
	// 沿着旋转方向X轴前进，即面向的前方
	// Pitch & Roll 设置为0，可获得水平面的Forward，即XOY平面
	ControlRot.Pitch = ControlRot.Roll = 0.0f;
	// ControlRot.Vector()返回值为当前FRotator旋转面向的方向向量
	this->AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = this->GetControlRotation();
	// 来自于Pawn
	// 方向，Scale值, 是否启用力作用
	// 沿着旋转方向X轴前进，即面向的前方
	ControlRot.Pitch = ControlRot.Roll = 0.0f;

	// 将ControlRot欧拉角转为矩阵，然后获得Y轴的方向轴，即Right
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	this->AddMovementInput(RightVector, value);
}

void ASCharacter::SprintStart()
{
	this->ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	this->ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryAttack()
{
	if (this->ActionComp->StartActionByName(this, "PrimaryAttack")) {
		//// 转向攻击方向
		//FRotator rot = this->GetControlRotation();
		//rot.Pitch = rot.Roll = 0;
		//this->SetActorRotation(rot);
	}

	//// 之前的没有结束, 即上一次的Spawn之后才能进行下一次攻击
	//if (TimerHandle_Attack.IsValid())
	//	return;
	//// PlayerCharacter -> Attack -> AttackAnim -> "Primary_Attack_Medium"
	//// 播放攻击动画
	//PlayAnimMontage(AttackAnim);
	//// 转向攻击方向
	//FRotator rot = this->GetControlRotation();
	//rot.Pitch = rot.Roll = 0;
	//this->SetActorRotation(rot);

	//// 设置定时器，匹配攻击动画进行Spawn
	//// 暂时这么做，正解应该在动画上制作
	//StartAttackEffects();
	//GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &ASCharacter::PrimaryAttack_TimeElapsed, this->AttackAnimDelay);
	////GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

//void ASCharacter::PrimaryAttack_TimeElapsed()
//{
//	//// 获取Mesh -> 获取Socket的位置
//	//FVector HandleLocation = this->GetMesh()->GetSocketLocation("Muzzle_01");
//
//	//// 生成点的Transform
//	//// Actor的旋转方向，即PlayerCharacter
//	////FTransform SpawnTM = FTransform(this->GetActorRotation(), HandleLocation);
//	//// Controller的旋转方向，即面向的方向，绑定了Camera,所以可以当作是Camera的方向
//	//FTransform SpawnTM = FTransform(this->GetControlRotation(), HandleLocation);
//
//	//// 生成的可选参数
//	//FActorSpawnParameters SpawnParams;
//	//// 生成的碰撞的覆盖处理
//	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	//
//	//// 设置生成物的 Instigator
//	//SpawnParams.Instigator = this;
//
//	//// 场景世界 -> 创建Actor
//	//// 所有能在Scene生成的物体都必须继承Actor
//	//// 生成的类，Transform(生成的位置/旋转/Scale)，可选参数
//	//GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
//
//	//// Clear TimeHandle
//	//TimerHandle_PrimaryAttack.Invalidate();
//	ASMagicProjectile* Proj = Cast<ASMagicProjectile>(SpawnProjectile(ProjectileClass));
//	if(Proj)
//		Proj->SetDamageAmount(this->ProjectileDamageAmount);
//}

void ASCharacter::BlackHoleAttack()
{
	if (ActionComp->StartActionByName(this, "BlackHole")) {
		//// 转向攻击方向
		//FRotator rot = this->GetControlRotation();
		//rot.Pitch = rot.Roll = 0;
		//this->SetActorRotation(rot);
	}
	//// 之前的没有结束, 即上一次的Spawn之后才能进行下一次攻击
	//if (TimerHandle_Attack.IsValid())
	//	return;
	//// PlayerCharacter -> Attack -> AttackAnim -> "Primary_Attack_Medium"
	//// 播放攻击动画
	//PlayAnimMontage(AttackAnim);
	//// 转向攻击方向
	//FRotator rot = this->GetControlRotation();
	//rot.Pitch = rot.Roll = 0;
	//this->SetActorRotation(rot);

	//StartAttackEffects();
	//GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &ASCharacter::BlackholeAttack_TimeElapsed, AttackAnimDelay);
}
//
//void ASCharacter::BlackholeAttack_TimeElapsed()
//{
//	SpawnProjectile(BlackHoleProjectileClass);
//}

void ASCharacter::Dash()
{
	if (ActionComp->StartActionByName(this, "Dash")) {
		//// 转向攻击方向
		//FRotator rot = this->GetControlRotation();
		//rot.Pitch = rot.Roll = 0;
		//this->SetActorRotation(rot);
	}
	//// 之前的没有结束, 即上一次的Spawn之后才能进行下一次攻击
	//if (TimerHandle_Attack.IsValid())
	//	return;
	//// PlayerCharacter -> Attack -> AttackAnim -> "Primary_Attack_Medium"
	//// 播放攻击动画
	//PlayAnimMontage(AttackAnim);
	//// 转向攻击方向
	//FRotator rot = this->GetControlRotation();
	//rot.Pitch = rot.Roll = 0;
	//this->SetActorRotation(rot);

	//StartAttackEffects();
	//GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &ASCharacter::Dash_TimeElapsed, AttackAnimDelay);
}
//
//void ASCharacter::Dash_TimeElapsed()
//{
//	SpawnProjectile(DashProjectileClass);
//}
//
//void ASCharacter::StartAttackEffects()
//{
//	PlayAnimMontage(AttackAnim);
//
//	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
//}

void ASCharacter::PrimaryInteract()
{
	// 调用交互函数
	if (this->InteractionComp) {
		this->InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// 受伤
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// Rage added equal to damage received (Abs to turn into positive rage number)
		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);
	}

	if (NewHealth <= 0.0f && Delta < 0.0f) {
		// 禁用输入
		APlayerController* PC = Cast<APlayerController>(this->GetController());

		this->DisableInput(PC);
		//// Ragdoll
		//// 启用物理学模拟
		//this->GetMesh()->SetAllBodiesSimulatePhysics(true);
		//// 设置碰撞为Ragdoll,UE自带
		//this->GetMesh()->SetCollisionProfileName("Ragdoll");
		////禁用移动
		//this->GetCharacterMovement()->DisableMovement();
		// 禁用碰撞检测
		this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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

	// 禁用 PlayerCharacter -> Pawn下的Use Controller Rotation
	// 启用 SpringArmComp -> CameraSettings -> Use Pawn Control Rotation
	// Or 脚本更改
	// 保证player不随输入旋转，随着移动旋转，Camera随着弹簧臂旋转
	//this->bUseControllerRotationPitch = false;
	//this->bUseControllerRotationYaw = false;
	//this->bUseControllerRotationRoll = false;
	//SpringArmComp->bUsePawnControlRotation = true;
	//this->GetCharacterMovement()->bOrientRotationToMovement = true;

	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);

	// 绑定Action
	// ActionString, TriggerStatus, UserObject, InvokeFunction
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	// Used generic name 'SecondaryAttack' for binding
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}
//
//AActor* ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
//{
//	// Clear TimeHandle
//	TimerHandle_Attack.Invalidate();
//	// ClassToSpwan 有效
//	// ensureAlways -> assert
//	if (ensureAlways(ClassToSpawn))
//	{
//		// 右手 socket point
//		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
//
//		FActorSpawnParameters SpawnParams;
//		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		SpawnParams.Instigator = this;
//
//		FHitResult Hit;
//		// 摄像机位置 -> 屏幕中心点的世界坐标
//		FVector TraceStart = CameraComp->GetComponentLocation();
//		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
//		// controller 控制 actor,所以选用 GetControlRotation() 获取实际旋转值
//		// PlayerCharacter ->Pawn -> User Controller Rotation
//		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 10000);
//
//		FCollisionShape Shape;
//		Shape.SetSphere(20.0f);
//
//		// Ignore Player
//		FCollisionQueryParams Params;
//		Params.AddIgnoredActor(this);
//
//		FCollisionObjectQueryParams ObjParams;
//		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
//		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
//		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
//
//		ObjParams.AddObjectTypesToQuery(ECC_Vehicle);
//		ObjParams.AddObjectTypesToQuery(ECC_PhysicsBody);
//
//		FRotator ProjRotation;
//		// true if we got to a blocking hit (Alternative: SweepSingleByChannel with ECC_WorldDynamic)
//		// 教程项目使用 SweepSingleByObjectType
//		//if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
//		if (GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjParams))
//		{
//			// Adjust location to end up at crosshair look-at
//			// 教程使用 ->等效
//			//ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
//			ProjRotation = (Hit.ImpactPoint - HandLocation).Rotation();
//
//		}
//		else
//		{
//			// Fall-back since we failed to find any blocking hit
//			// 教程使用
//			//ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
//			ProjRotation = (TraceEnd - HandLocation).Rotation();
//		}
//
//		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
//		return GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
//	}
//
//	return nullptr;
//}

void ASCharacter::PlayerHeal(float Amount /* = 100 */)
{
	this->AttributeComp->ApplyHealthChange(this, Amount);
}

void ASCharacter::PlayerSuicide()
{
	USGameplayFunctionLibrary::ApplyDamage(this, this, this->AttributeComp->GetHealthMax());
}

UCameraComponent* ASCharacter::GetCameraComp()
{
	return this->CameraComp;
}

void ASCharacter::ServerSetControlRotation_Implementation(FRotator rot)
{
	this->ControlRotation = rot;
}

void ASCharacter::AddControllerPitchInput(float Val)
{
	Super::AddControllerPitchInput(Val);
	ServerSetControlRotation(this->GetControlRotation());
}

void ASCharacter::AddControllerYawInput(float Val)
{
	Super::AddControllerYawInput(Val);
	ServerSetControlRotation(this->GetControlRotation());
}

void ASCharacter::AddControllerRollInput(float Val)
{
	Super::AddControllerRollInput(Val);
	ServerSetControlRotation(this->GetControlRotation());
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return this->CameraComp->GetComponentLocation();
}


void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, ControlRotation);
}

FGuid ASCharacter::NewGuid()
{
	static int32 FixedSeed = 555;

	// 创建随机数生成器，使用固定的种子
	static FRandomStream RandomStream(FixedSeed);
	// 根据当前索引生成一个 GUID
	uint32 A = RandomStream.RandRange(0, 0x7FFFFFFE);
	uint32 B = RandomStream.RandRange(0, 0x7FFFFFFE);
	uint32 C = RandomStream.RandRange(0, 0x7FFFFFFE);
	uint32 D = RandomStream.RandRange(0, 0x7FFFFFFE);
	//UE_LOG(LogTemp, Warning, TEXT("Generated Random Components: A = %u, B = %u, C = %u, D = %u"), A, B, C, D);
	FGuid GeneratedGuid = FGuid(A, B, C, D);

	return GeneratedGuid;
}

