// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Others/NPCStatusBar.h"

#include "Components/ProgressBar.h"

void UNPCStatusBar::UpdateHealthBar(const float& CurrentHealth, const float& MaxHealth)
{
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
}
