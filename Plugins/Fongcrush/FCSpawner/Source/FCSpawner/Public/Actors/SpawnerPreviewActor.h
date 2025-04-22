// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SpawnerPreviewActor.generated.h"

UCLASS()
class FCSPAWNER_API ASpawnerPreviewActor : public AActor
{
	GENERATED_BODY()
	
	friend class USpawnerComponent;
	
public:
	ASpawnerPreviewActor();

	virtual void PostInitProperties() override;
	virtual void Destroyed() override;

	virtual void Tick(float DeltaSeconds) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
	virtual void PushSelectionToProxies() override;
	
	FORCEINLINE USpawnerComponent* GetOwnSpawner() { return OwnerSpawner; }
	
private:
	UPROPERTY()
	TObjectPtr<USpawnerComponent> OwnerSpawner;
};