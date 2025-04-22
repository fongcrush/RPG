// Fill out your copyright notice in the Description page of Project Settings.

#include "FCBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UFCBlueprintLibrary::K2_CancelAbility(UAbilitySystemComponent* AbilitySystemComp, TSubclassOf<UGameplayAbility> Ability)
{
	if (AbilitySystemComp && Ability)
	{
		AbilitySystemComp->CancelAbility(Ability.GetDefaultObject());
	}
}
