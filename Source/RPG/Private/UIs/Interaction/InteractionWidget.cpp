﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Interaction/InteractionWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Interfaces/Interaction.h"


void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f;
}

void UInteractionWidget::UpdateWidget(const FInteractableData& InteractableData)
{
	switch (InteractableData.InteractableType)
	{
	case EInteractableType::Pickup:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if (InteractableData.Quantity <= 1)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"), InteractableData.Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;
		
	case EInteractableType::NPC:
		break;
		
	case EInteractableType::Toggle:
		break;
		
	case EInteractableType::Container:
		break;
		
	case EInteractableType::Device:
		break;
	}

	ActionText->SetText(InteractableData.InteractionText);
	NameText->SetText(InteractableData.Name);
}

float UInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}
