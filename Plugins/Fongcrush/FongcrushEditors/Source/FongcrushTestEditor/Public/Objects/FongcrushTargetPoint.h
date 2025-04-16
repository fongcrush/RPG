// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "FongcrushTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class FONGCRUSHTESTEDITOR_API AFongcrushTargetPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Points")
	TArray<FVector> Points;
};
