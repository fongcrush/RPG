// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCExampleEditorActor.generated.h"

UCLASS()
class FCEXAMPLEEDITOR_API AFCExampleEditorActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "테스트")
	bool bOption1 = false;
	
	UPROPERTY(EditAnywhere, Category = "테스트")
	bool bOption2 = false;
	
	UPROPERTY(EditAnywhere, Category = "테스트")
	int Num = 0;
};
