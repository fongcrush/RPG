// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleListeners/FongcrushTapListenerBase.h"
#include "FongcrushTestEditor.h"


void FFongcrushTapListenerBase::OnStartupModule()
{
	Initialize();
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TabName, FOnSpawnTab::CreateRaw(this, &FFongcrushTapListenerBase::SpawnTab))
	.SetGroup(FFongcrushTestEditor::Get().GetMenuRoot())
	.SetDisplayName(TabDisplayName)
	.SetTooltipText(ToolTipText);
}

void FFongcrushTapListenerBase::OnShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
}
