// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModuleListeners/FongcrushTapListenerBase.h"

/**
 * 
 */
class FONGCRUSHTESTEDITOR_API FFongcrushTestTabListener : public FFongcrushTapListenerBase
{
public:
	virtual void OnStartupModule() override;
	virtual void Initialize() override;
	virtual TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& TapSpawnArgs) override;
};
