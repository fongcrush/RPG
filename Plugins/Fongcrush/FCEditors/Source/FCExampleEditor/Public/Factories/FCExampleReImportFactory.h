// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCExampleImportFactory.h"
#include "EditorReimportHandler.h"
#include "FCExampleReImportFactory.generated.h"

/**
 * 
 */
UCLASS()
class FCEXAMPLEEDITOR_API UFCExampleReImportFactory : public UFCExampleImportFactory, public FReimportHandler
{
	GENERATED_BODY()

	// FReimportHandler interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	// ~ FReimportHandler interface
};
