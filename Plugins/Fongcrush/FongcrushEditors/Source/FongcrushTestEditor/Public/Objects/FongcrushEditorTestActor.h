// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FongcrushEditorTestActor.generated.h"

UCLASS()
class FONGCRUSHTESTEDITOR_API AFongcrushEditorTestActor : public AActor
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
