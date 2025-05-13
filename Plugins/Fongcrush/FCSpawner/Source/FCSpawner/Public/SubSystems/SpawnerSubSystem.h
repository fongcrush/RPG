// Fill out your copyright notice in the Description page of Project Settings.
#if WITH_EDITOR

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SpawnerSubSystem.generated.h"

class USpawnerComponent;

/**
 * 
 */
UCLASS()
class FCSPAWNER_API USpawnerSubSystem : public UEngineSubsystem
{
	GENERATED_BODY()

	friend class USpawnerComponent;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static FString GetWorldTypeString(UObject* const& Object);
	
public:
	int32 GetSelectedSpawners(TArray<UObject*>& OutSelected) const;

#if WITH_EDITORONLY_DATA
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	TSet<TWeakObjectPtr<USpawnerComponent>> Spawners;

	TMap<UPrimitiveComponent*, TWeakObjectPtr<USpawnerComponent>> PreviewMap;
#endif
};


#endif
