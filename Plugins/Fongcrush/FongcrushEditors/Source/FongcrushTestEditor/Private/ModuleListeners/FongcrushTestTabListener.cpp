// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleListeners/FongcrushTestTabListener.h"
#include "FongcrushTestEditor.h"
#include "Slates/FongcrushTabWidget.h"


void FFongcrushTestTabListener::OnStartupModule()
{
	FFongcrushTapListenerBase::OnStartupModule();
	FFongcrushTestEditor::Get().AddMenuExtension(
		FName("Section_2"),
		FMenuExtensionDelegate::CreateRaw(this, &FFongcrushTestTabListener::MakeMenuEntry));
}

void FFongcrushTestTabListener::Initialize()
{
	TabName = "TestTab";
	TabDisplayName = FText::FromString(TEXT("테스트 탭"));
	ToolTipText = FText::FromString("Tab tool Test window");
}

TSharedRef<SDockTab> FFongcrushTestTabListener::SpawnTab(const FSpawnTabArgs& TapSpawnArgs)
{
	TSharedRef<SDockTab> SpawnedTab =
		SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SFongcrushTabWidget)
			.Tool(SharedThis(this))
		];

	return SpawnedTab;
}
