// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Tasks/AbilityTask_WaitMovementModeNotEqual.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// #include UE_INLINE_GENERATED_CPP_BY_NAME(UAbilityTask_WaitMovementModeNotEqual)

UAbilityTask_WaitMovementModeNotEqual::UAbilityTask_WaitMovementModeNotEqual()
{
	RequiredMode = MOVE_None;
}

UAbilityTask_WaitMovementModeNotEqual* UAbilityTask_WaitMovementModeNotEqual::CreateWaitMovementModeNotEqual(
	UGameplayAbility* OwningAbility,
	const EMovementMode TargetMode)
{
	UAbilityTask_WaitMovementModeNotEqual* MyObj = NewAbilityTask<UAbilityTask_WaitMovementModeNotEqual>(OwningAbility);
	MyObj->RequiredMode = TargetMode;
	return MyObj;
}

void UAbilityTask_WaitMovementModeNotEqual::Activate()
{
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
	{
		Character->MovementModeChangedDelegate.AddDynamic(
			this,
			&UAbilityTask_WaitMovementModeNotEqual::OnMovementModeChange
		);
		MyCharacter = Character;
		SetWaitingOnAvatar();
	}
	else
	{
		EndTask();
	}
}

void UAbilityTask_WaitMovementModeNotEqual::OnMovementModeChange(
	ACharacter* Character,
	EMovementMode PrevMovementMode,
	uint8 PreviousCustomMode)
{
	if (Character)
	{
		if (UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(Character->GetMovementComponent()))
		{
			if (MoveComp->MovementMode != RequiredMode)
			{
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnChange.Broadcast(MoveComp->MovementMode);
				}
				EndTask();
				return;
			}
		}
	}
}

void UAbilityTask_WaitMovementModeNotEqual::OnDestroy(bool AbilityEnded)
{
	if (MyCharacter.IsValid())
	{
		MyCharacter->MovementModeChangedDelegate.RemoveDynamic(
			this,
			&UAbilityTask_WaitMovementModeNotEqual::OnMovementModeChange
		);
	}
	
	Super::OnDestroy(AbilityEnded);
}
