// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModuleListeners/FCExampleTapListenerBase.h"

/**
 * 
 */
class FCEXAMPLEEDITOR_API FFCExampleTabListener : public FFCExampleTapListenerBase
{
public:
	virtual void OnStartupModule() override;
	virtual void Initialize() override;
	virtual TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& TapSpawnArgs) override;
};
