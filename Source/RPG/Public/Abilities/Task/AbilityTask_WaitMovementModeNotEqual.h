// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitMovementModeNotEqual.generated.h"

class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMovementModeNotEqualDelegate, EMovementMode, NewMovementMode);

UCLASS()
class RPG_API UAbilityTask_WaitMovementModeNotEqual : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_WaitMovementModeNotEqual();
	
	/** Wait until movement mode changes (E.g., landing) */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE", DisplayName="WaitMovementModeNotEqual"))
	static UAbilityTask_WaitMovementModeNotEqual* CreateWaitMovementModeNotEqual(UGameplayAbility* OwningAbility, EMovementMode TargetMode);
	
	virtual void Activate() override;
	
	UFUNCTION()
	void OnMovementModeChange(ACharacter * Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
	
private:
	virtual void OnDestroy(bool AbilityEnded) override;

public:
	UPROPERTY(BlueprintAssignable)
	FMovementModeNotEqualDelegate OnChange;

	EMovementMode RequiredMode;
private:

	TWeakObjectPtr<ACharacter>	MyCharacter;
};
