// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/RPGCharacterBase.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class RPG_API AEnemyCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
