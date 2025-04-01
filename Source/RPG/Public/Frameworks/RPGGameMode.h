// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGGameMode.generated.h"

class UObjectPoolComponent;
/**
 * 
 */
UCLASS()
class RPG_API ARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARPGGameMode();
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	FORCEINLINE UObjectPoolComponent* GetObjectPoolComponent() const { return ObjectPoolComponent; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PublicData")
	TObjectPtr<UObjectPoolComponent> ObjectPoolComponent;
};
