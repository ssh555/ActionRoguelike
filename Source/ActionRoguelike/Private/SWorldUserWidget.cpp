// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Components/SizeBox.h>
#include "SAttributeComponent.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		//UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget."));
		return;
	}

	// AttachedActor 对应的 ScreePos
	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(this->GetOwningPlayer(), this->AttachedActor->GetActorLocation() + this->WorldOffset, ScreenPosition)) {
		// Viewport的缩放比例不同，会影响ScreenPos
		ScreenPosition /= UWidgetLayoutLibrary::GetViewportScale(this);

		if (this->ParentSizeBox) {
			// 放置于 ScreenPosition
			this->ParentSizeBox->SetRenderTranslation(ScreenPosition);

		}
	}
}
