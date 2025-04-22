// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FCBlueprintLibrary.generated.h"

/**
 * FC's blueprint library
 */
UCLASS(meta=(ScriptName="FCLibrary", BlueprintThreadSafe), MinimalAPI)
class UFCBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Find & return UClass
	 * 
	 * @param Class 찾으려는 UClass
	 */
	UFUNCTION(BlueprintPure, Category = "FC", meta = (DisplayName = "Class Reference"))
	static TSubclassOf<UObject> K2_GetUClass(const TSubclassOf<UObject>& Class) { return Class; }
	
	/**
	 * 
	 * @param AbilitySystemComp 
	 * @param Ability 
	 */
	UFUNCTION(BlueprintCallable, Category = "FC", meta = (DisplayName = "Cancel Ability"))
	static void K2_CancelAbility(class UAbilitySystemComponent* AbilitySystemComp, TSubclassOf<class UGameplayAbility> Ability);
};
