// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCExampleFactory.generated.h"

/**
 * UFactory - 애샛 생성기, 에셋 생성에 사용되는 팩토리 클래스
 * 
 */
UCLASS()
class FCEXAMPLEEDITOR_API UFCExampleFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UFCExampleFactory();
	virtual UObject* FactoryCreateNew(
		UClass* Class,
		UObject* InParent,
		FName Name,
		EObjectFlags Flags,
		UObject* Context,
		FFeedbackContext* Warn) override;
};
