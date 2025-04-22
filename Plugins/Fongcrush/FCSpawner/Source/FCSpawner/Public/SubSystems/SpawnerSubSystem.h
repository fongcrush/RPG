// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SpawnerSubSystem.generated.h"

class ASpawnerPreviewActor;
class USpawnerComponent;

/**
 * 
 */
UCLASS()
class FCSPAWNER_API USpawnerSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

	friend class USpawnerComponent;
	friend class ASpawnerPreviewActor;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override { return WorldType == EWorldType::Editor; }	

	FORCEINLINE TSet<USpawnerComponent*>& GetSpawnerComps() { return SpawnerComps; }
	FORCEINLINE TSet<ASpawnerPreviewActor*>& GetPreviewActors() { return PreviewActors; }
	

private:	
	UPROPERTY()
	TSet<USpawnerComponent*> SpawnerComps;
	
	UPROPERTY()
	TSet<ASpawnerPreviewActor*> PreviewActors;
};
