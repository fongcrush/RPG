// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCExampleData.generated.h"

/**
 * 
 */
UCLASS()
class FCEXAMPLEEDITOR_API UFCExampleData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "테스트")
	FString TestString;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "테스트")
	FString SourceFilePath;
#endif
};
