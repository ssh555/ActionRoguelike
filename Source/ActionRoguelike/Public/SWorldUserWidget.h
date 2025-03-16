// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttachedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector WorldOffset;

protected:
	// Palette -> Size Box -> SetName("ParentSizeBox")
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;
	
	/// <summary>
	/// Ã¿Ö¡µ÷ÓÃ
	/// </summary>
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
