// Fill out your copyright notice in the Description page of Project Settings.

// User Defined
#include "UIs/Others/MainWidget.h"
//UE
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"

void UMainWidget::UpdateHealthBar(const float& CurrentHealth, const float& MaxHealth) const
{
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(HealthBar->Slot))
	{
		CanvasSlot->SetSize(FVector2D(MaxHealth * 2, 10.f));
	}
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
}

void UMainWidget::UpdateManaBar(const float& CurrentMana, const float& MaxMana) const
{
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ManaBar->Slot))
	{
		CanvasSlot->SetSize(FVector2D(MaxMana * 2, 10.f));
	}
	ManaBar->SetPercent(CurrentMana / MaxMana);
}

void UMainWidget::UpdateStaminaBar(const float& CurrentStamina, const float& MaxStamina) const
{
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(StaminaBar->Slot))
	{
		CanvasSlot->SetSize(FVector2D(MaxStamina * 2, 10.f));
	}
	StaminaBar->SetPercent(CurrentStamina / MaxStamina);
}
