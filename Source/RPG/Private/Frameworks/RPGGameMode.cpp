// Fill out your copyright notice in the Description page of Project Settings.


#include "Frameworks/RPGGameMode.h"
#include "Components/Others/ObjectPoolComponent.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/HUD.h"
#include "Misc/DataValidation.h"


ARPGGameMode::ARPGGameMode()
{
	ObjectPoolComponent = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ObjectPoolComponent"));
}

EDataValidationResult ARPGGameMode::IsDataValid(FDataValidationContext& Context) const
{
	if (PlayerControllerClass == nullptr)
	{
		Context.AddError(FText::FromString(TEXT("PlayerController 클래스 바인딩 필요!")));		
	}
	if (DefaultPawnClass == nullptr)
	{
		Context.AddError(FText::FromString(TEXT("DefaultPawn 클래스 바인딩 필요!")));
	}
	if (HUDClass == nullptr)
	{
		Context.AddError(FText::FromString(TEXT("HUD 클래스 바인딩 필요!")));
	}
	if (GameStateClass == nullptr)
	{
		Context.AddError(FText::FromString(TEXT("GameState 클래스 바인딩 필요!")));
	}
	return Super::IsDataValid(Context);
}
