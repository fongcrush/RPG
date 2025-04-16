// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Spawner.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USpawner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API ISpawner
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void StartSpawner() {};

	UFUNCTION()
	virtual void Spawn() {}

	UFUNCTION()
	virtual void Interupt() {}
};
