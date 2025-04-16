// Fill out your copyright notice in the Description page of Project Settings.

#include "FongcrushBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UFongcrushBlueprintLibrary::K2_CancelAbility(UAbilitySystemComponent* AbilitySystemComp, TSubclassOf<UGameplayAbility> Ability)
{
	if (AbilitySystemComp && Ability)
	{
		AbilitySystemComp->CancelAbility(Ability.GetDefaultObject());
	}
}
