// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"
#include "../Public/SAttributeComponent.h"

#include "NetDataWriter.h"
#include "NetDataReader.h"
#include "Struct/NPStruct.h"
#include <NetPacketProcessor.h>
#include <example.hpp>
#include <Struct/NPBPFunctionLibrary.h>



bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp) {
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount)) {
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		// 启用物理模拟
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName)) {
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}

void USGameplayFunctionLibrary::TestNP(const Ftemplate_ue& t, Ftemplate_ue& r)
{
	NetPacket::NetDataWriter writer;
	NetPacket::NetDataReader reader;

	t.Serialize(writer);
	reader.SetSource(writer.CopyData(), writer.Length());
	r.Deserialize(reader);
	delete[] reader.GetRawData();

	processor.Receive(const_cast<uint8_t*>(writer.Data()), writer.Length(), false);
	processor.Process();
}

void USGameplayFunctionLibrary::TestNPDelegate(UStruct* stype, FRegisterProcessDelegate Delegate)
{
	UNPBPFunctionLibrary::Register(processor, stype, Delegate);
}

NetPacket::NetPacketProcessor USGameplayFunctionLibrary::processor;
