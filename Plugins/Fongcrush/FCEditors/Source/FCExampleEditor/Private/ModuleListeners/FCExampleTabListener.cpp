// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleListeners/FCExampleTabListener.h"
#include "FCExampleEditor.h"
#include "Slates/FCExampleTabWidget.h"


void FFCExampleTabListener::OnStartupModule()
{
	FFCExampleTapListenerBase::OnStartupModule();
	FFCExampleEditor::Get().AddMenuExtension(
		FName("Section_2"),
		FMenuExtensionDelegate::CreateRaw(this, &FFCExampleTabListener::MakeMenuEntry));
}

void FFCExampleTabListener::Initialize()
{
	TabName = "TestTab";
	TabDisplayName = FText::FromString(TEXT("테스트 탭"));
	ToolTipText = FText::FromString("Tab tool Test window");
}

TSharedRef<SDockTab> FFCExampleTabListener::SpawnTab(const FSpawnTabArgs& TapSpawnArgs)
{
	TSharedRef<SDockTab> SpawnedTab =
		SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SFCExampleTabWidget)
			.Tool(SharedThis(this))
		];

	return SpawnedTab;
}
