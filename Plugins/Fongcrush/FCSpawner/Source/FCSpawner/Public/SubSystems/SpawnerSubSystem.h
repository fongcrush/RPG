// Fill out your copyright notice in the Description page of Project Settings.
#if WITH_EDITOR

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SpawnerSubSystem.generated.h"

class USpawnerComponent;


/**
 * Transaction용 UObject
 * Subsystem 자체는 Transaction을 지원하지 않는다.
 */
UCLASS()
class FCSPAWNER_API USpawnerData : public UObject
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
public:
	UPROPERTY()
	TSet<TWeakObjectPtr<USpawnerComponent>> SpawnerComps;

	UPROPERTY()
	TMap<UPrimitiveComponent*, TWeakObjectPtr<USpawnerComponent>> PreviewCompMap;
#endif
};

/**
 * 
 */
UCLASS()
class FCSPAWNER_API UEditorWorldSpawnerSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

	friend class USpawnerComponent;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
public:
	UEditorWorldSpawnerSubSystem();

private:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override
	{
		return WorldType == EWorldType::Editor || WorldType == EWorldType::EditorPreview;
	}
	
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
