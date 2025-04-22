// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FCSPAWNEREDITOR_API FFCSpawnerModeToolkit : public FModeToolkit
{
protected:
	/** IToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode) override;
	virtual FName GetToolkitFName() const override { return FName("FCSpawnerModeToolkit"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("BuilderModeToolkit", "DisplayName", "Builder"); }
	virtual FEdMode* GetEditorMode() const override final;

public:
	FORCEINLINE TSharedPtr<IDetailsView> GetDetailView() const { return DetailsView; }
};
