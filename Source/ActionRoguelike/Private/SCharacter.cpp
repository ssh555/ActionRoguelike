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
	1. ��C++���� => new
	2. UObject(U ��ͷ => NewObject
	3. Actor(A ��ͷ => World->SpawnActor
	4. Component => CreateDefaultSubobject
	5. UE��Դ => StaticLoadObject
	6. �����ļ���Դ => ���� Load
*/

// Sets default values
ASCharacter::ASCharacter()
{
	// �����Ƿ����� Tick
	this->PrimaryActorTick.bCanEverTick = false;

	// ��������
	// �����ļ�ʵ�ʴ���λ�� Classes/[include]
	this->SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");

	this->ControlRotation = this->GetControlRotation();
	// ���帽�ţ�child->parent
	// RootComponent ������ĸ����,����νṹ�еĵ�һ�����
	this->SpringArmComp->SetupAttachment(this->RootComponent);
	// �� ASCharacter::SetupPlayerInputComponent
	SpringArmComp->bUsePawnControlRotation = true;

	this->CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	this->CameraComp->SetupAttachment(this->SpringArmComp);

	// �� ASCharacter::SetupPlayerInputComponent
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationRoll = false;
	this->GetCharacterMovement()->bOrientRotationToMovement = true;

	//// ����PrimaryAttackObject��SpawnClass
	//// LoadClass<T>
	//// Or ��������� PlayerCharacter -> SCharacter -> ProjectileClass
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
	����ϵ
	X -> Forward
	Y -> Right
	Z -> Up
*/

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = this->GetControlRotation();
	// ������Pawn
	// ����Scaleֵ, �Ƿ�����������
	// ������ת����X��ǰ�����������ǰ��
	// Pitch & Roll ����Ϊ0���ɻ��ˮƽ���Forward����XOYƽ��
	ControlRot.Pitch = ControlRot.Roll = 0.0f;
	// ControlRot.Vector()����ֵΪ��ǰFRotator��ת����ķ�������
	this->AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = this->GetControlRotation();
	// ������Pawn
	// ����Scaleֵ, �Ƿ�����������
	// ������ת����X��ǰ�����������ǰ��
	ControlRot.Pitch = ControlRot.Roll = 0.0f;

	// ��ControlRotŷ����תΪ����Ȼ����Y��ķ����ᣬ��Right
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
		//// ת�򹥻�����
		//FRotator rot = this->GetControlRotation();
		//rot.Pitch = rot.Roll = 0;
		//this->SetActorRotation(rot);
	}

	//// ֮ǰ��û�н���, ����һ�ε�Spawn֮����ܽ�����һ�ι���
	//if (TimerHandle_Attack.IsValid())
	//	return;
	//// PlayerCharacter -> Attack -> AttackAnim -> "Primary_Attack_Medium"
	//// ���Ź�������
	//PlayAnimMontage(AttackAnim);
	//// ת�򹥻�����
	//FRotator rot = this->GetControlRotation();
	//rot.Pitch = rot.Roll = 0;
	//this->SetActorRotation(rot);

	//// ���ö�ʱ����ƥ�乥����������Spawn
	//// ��ʱ��ô��������Ӧ���ڶ���������
	//StartAttackEffects();
	//GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &ASCharacter::PrimaryAttack_TimeElapsed, this->AttackAnimDelay);
	////GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

//void ASCharacter::PrimaryAttack_TimeElapsed()
//{
//	//// ��ȡMesh -> ��ȡSocket��λ��
//	//FVector HandleLocation = this->GetMesh()->GetSocketLocation("Muzzle_01");
//
//	//// ���ɵ��Transform
//	//// Actor����ת���򣬼�PlayerCharacter
//	////FTransform SpawnTM = FTransform(this->GetActorRotation(), HandleLocation);
//	//// Controller����ת���򣬼�����ķ��򣬰���Camera,���Կ��Ե�����Camera�ķ���
//	//FTransform SpawnTM = FTransform(this->GetControlRotation(), HandleLocation);
//
//	//// ���ɵĿ�ѡ����
//	//FActorSpawnParameters SpawnParams;
//	//// ���ɵ���ײ�ĸ��Ǵ���
//	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	//
//	//// ����������� Instigator
//	//SpawnParams.Instigator = this;
//
//	//// �������� -> ����Actor
//	//// ��������Scene���ɵ����嶼����̳�Actor
//	//// ���ɵ��࣬Transform(���ɵ�λ��/��ת/Scale)����ѡ����
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
		//// ת�򹥻�����
		//FRotator rot = this->GetControlRotation();
		//rot.Pitch = rot.Roll = 0;
		//this->SetActorRotation(rot);
	}
	//// ֮ǰ��û�н���, ����һ�ε�Spawn֮����ܽ�����һ�ι���
	//if (TimerHandle_Attack.IsValid())
	//	return;
	//// PlayerCharacter -> Attack -> AttackAnim -> "Primary_Attack_Medium"
	//// ���Ź�������
	//PlayAnimMontage(AttackAnim);
	//// ת�򹥻�����
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
		//// ת�򹥻�����
		//FRotator rot = this->GetControlRotation();
		//rot.Pitch = rot.Roll = 0;
		//this->SetActorRotation(rot);
	}
	//// ֮ǰ��û�н���, ����һ�ε�Spawn֮����ܽ�����һ�ι���
	//if (TimerHandle_Attack.IsValid())
	//	return;
	//// PlayerCharacter -> Attack -> AttackAnim -> "Primary_Attack_Medium"
	//// ���Ź�������
	//PlayAnimMontage(AttackAnim);
	//// ת�򹥻�����
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
	// ���ý�������
	if (this->InteractionComp) {
		this->InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// ����
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// Rage added equal to damage received (Abs to turn into positive rage number)
		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);
	}

	if (NewHealth <= 0.0f && Delta < 0.0f) {
		// ��������
		APlayerController* PC = Cast<APlayerController>(this->GetController());

		this->DisableInput(PC);
		//// Ragdoll
		//// ��������ѧģ��
		//this->GetMesh()->SetAllBodiesSimulatePhysics(true);
		//// ������ײΪRagdoll,UE�Դ�
		//this->GetMesh()->SetCollisionProfileName("Ragdoll");
		////�����ƶ�
		//this->GetCharacterMovement()->DisableMovement();
		// ������ײ���
		this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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

	// ���� PlayerCharacter -> Pawn�µ�Use Controller Rotation
	// ���� SpringArmComp -> CameraSettings -> Use Pawn Control Rotation
	// Or �ű�����
	// ��֤player����������ת�������ƶ���ת��Camera���ŵ��ɱ���ת
	//this->bUseControllerRotationPitch = false;
	//this->bUseControllerRotationYaw = false;
	//this->bUseControllerRotationRoll = false;
	//SpringArmComp->bUsePawnControlRotation = true;
	//this->GetCharacterMovement()->bOrientRotationToMovement = true;

	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);

	// ��Action
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
//	// ClassToSpwan ��Ч
//	// ensureAlways -> assert
//	if (ensureAlways(ClassToSpawn))
//	{
//		// ���� socket point
//		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
//
//		FActorSpawnParameters SpawnParams;
//		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		SpawnParams.Instigator = this;
//
//		FHitResult Hit;
//		// �����λ�� -> ��Ļ���ĵ����������
//		FVector TraceStart = CameraComp->GetComponentLocation();
//		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
//		// controller ���� actor,����ѡ�� GetControlRotation() ��ȡʵ����תֵ
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
//		// �̳���Ŀʹ�� SweepSingleByObjectType
//		//if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
//		if (GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjParams))
//		{
//			// Adjust location to end up at crosshair look-at
//			// �̳�ʹ�� ->��Ч
//			//ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
//			ProjRotation = (Hit.ImpactPoint - HandLocation).Rotation();
//
//		}
//		else
//		{
//			// Fall-back since we failed to find any blocking hit
//			// �̳�ʹ��
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

	// �����������������ʹ�ù̶�������
	static FRandomStream RandomStream(FixedSeed);
	// ���ݵ�ǰ��������һ�� GUID
	uint32 A = RandomStream.RandRange(0, 0x7FFFFFFE);
	uint32 B = RandomStream.RandRange(0, 0x7FFFFFFE);
	uint32 C = RandomStream.RandRange(0, 0x7FFFFFFE);
	uint32 D = RandomStream.RandRange(0, 0x7FFFFFFE);
	//UE_LOG(LogTemp, Warning, TEXT("Generated Random Components: A = %u, B = %u, C = %u, D = %u"), A, B, C, D);
	FGuid GeneratedGuid = FGuid(A, B, C, D);

	return GeneratedGuid;
}

