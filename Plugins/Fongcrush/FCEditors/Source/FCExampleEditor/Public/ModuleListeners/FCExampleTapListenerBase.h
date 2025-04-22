// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FCExampleModuleBase.h"

/**
 * 
 */
class FCEXAMPLEEDITOR_API FFCExampleTapListenerBase : public IModuleListener, public TSharedFromThis<FFCExampleTapListenerBase>
{
public:
	virtual void OnStartupModule() override;
	virtual void OnShutdownModule() override;
	virtual void Initialize() {};
	
	/** 탭 생성 */
	virtual TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& TapSpawnArgs) { return SNew(SDockTab); }
	/** 메뉴 항목 생성 */
	virtual void MakeMenuEntry(FMenuBuilder& MenuBuilder){ FGlobalTabmanager::Get()->PopulateTabSpawnerMenu(MenuBuilder, TabName); }
	
protected:
	FName TabName;
	FText TabDisplayName;
	FText ToolTipText;
};
