// Fill out your copyright notice in the Description page of Project Settings.


#include "InputTriggerTagQuery.h"
#include "AbilitySystemComponent.h"
#include "EnhancedPlayerInput.h"

ETriggerState UInputTriggerTagQuery::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	if (!PlayerInput) return ETriggerState::None;

	const APlayerController* PC = Cast<APlayerController>(PlayerInput->GetOuter());
	if (!PC) return ETriggerState::None;

	const APawn* Pawn = PC->GetPawn();
	if (!Pawn) return ETriggerState::None;

	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC) return ETriggerState::None;

	// 현재 게임플레이 태그 확인
	if (ASC->HasAnyMatchingGameplayTags(RequiredTagQuery))
	{
		return ETriggerState::Triggered; // 입력 허용
	}

	return ETriggerState::None;
}
