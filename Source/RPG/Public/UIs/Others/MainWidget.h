// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class RPG_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHealthBar(const float& CurrentHealth, const float& MaxHealth) const;
	void UpdateManaBar(const float& CurrentMana, const float& MaxMana) const;
	void UpdateStaminaBar(const float& CurrentStamina, const float& MaxStamina) const;

	FORCEINLINE UProgressBar* GetHealthBar() { return HealthBar; }
	FORCEINLINE UProgressBar* GetManaBar() { return ManaBar; }
	FORCEINLINE UProgressBar* GetStaminaBar() { return StaminaBar; }

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ManaBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
};
