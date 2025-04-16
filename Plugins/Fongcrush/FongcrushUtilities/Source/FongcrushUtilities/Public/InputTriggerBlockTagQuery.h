// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputTriggers.h"
#include "InputTriggerBlockTagQuery.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "BlockTagQuery"))
class FONGCRUSHUTILITIES_API UInputTriggerBlockTagQuery : public UInputTrigger
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tag Query")
	FGameplayTagContainer RequiredTagQuery;
	
protected:
	virtual ETriggerState
	UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;
};
