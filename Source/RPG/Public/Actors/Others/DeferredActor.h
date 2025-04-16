// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DeferredActor.generated.h"

UCLASS()
class RPG_API ADeferredActor : public AActor
{
	GENERATED_BODY()

public:
	ADeferredActor();
protected:
	virtual void BeginPlay() override;
	virtual void DeferredBegin();
	virtual void Tick(float DeltaTime) override;
};
