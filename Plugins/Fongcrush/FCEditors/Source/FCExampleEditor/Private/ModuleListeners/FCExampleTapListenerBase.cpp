// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleListeners/FCExampleTapListenerBase.h"
#include "FCExampleEditor.h"


void FFCExampleTapListenerBase::OnStartupModule()
{
	Initialize();
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TabName, FOnSpawnTab::CreateRaw(this, &FFCExampleTapListenerBase::SpawnTab))
	.SetGroup(FFCExampleEditor::Get().GetMenuRoot())
	.SetDisplayName(TabDisplayName)
	.SetTooltipText(ToolTipText);
}

void FFCExampleTapListenerBase::OnShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
}
