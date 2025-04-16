// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FongcrushImportFactory.h"
#include "EditorReimportHandler.h"
#include "FongcrushReImportFactory.generated.h"

/**
 * 
 */
UCLASS()
class FONGCRUSHTESTEDITOR_API UFongcrushReImportFactory : public UFongcrushImportFactory, public FReimportHandler
{
	GENERATED_BODY()

	// FReimportHandler interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	// ~ FReimportHandler interface
};
