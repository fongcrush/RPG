// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RPGCharacterBase.h"
#include "NonPlayableCharacter.generated.h"

UCLASS()
class RPG_API ANonPlayableCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()

public:
	ANonPlayableCharacter();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
