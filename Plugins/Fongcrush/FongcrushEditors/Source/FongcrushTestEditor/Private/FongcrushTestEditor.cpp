// Copyright Epic Games, Inc. All Rights Reserved.

#include "FongcrushTestEditor.h"

#include "Objects/FongcrushTestData.h"
#include "Objects/FongcrushEditorTestActor.h"
#include "AssetTypeActions/FongcrushDataTypeActions.h"
#include "ModuleListeners/FongcrushMenuListener.h"
#include "ModuleListeners/FongcrushTestTabListener.h"
#include "ModuleListeners/FongcrushEditorModeListener.h"
#include "DetailCustomizations/FongcrushTestActorDetail.h"
#include "LevelEditor.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FFongcrushTestEditor"

TSharedRef<FWorkspaceItem> FFongcrushTestEditor::MenuRoot = FWorkspaceItem::NewGroup(FText::FromString("Menu Root"));

void FFongcrushTestEditor::StartupModule()
{
	if (!IsRunningCommandlet())
	{
		// 레벨 에디터 메뉴 등록
		{
			// 레벨 에디터 모듈 로드
			FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
			// 레벨 에디터 메뉴 익스텐션 매니저
			LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
			// 메뉴 확장자 생성
			MenuExtender = MakeShared<FExtender>();
			// 상단바에 메뉴 추가
			MenuExtender->AddMenuBarExtension(
				FName("Window"),
				EExtensionHook::Before,
				nullptr,
				FMenuBarExtensionDelegate::CreateRaw(this, &FFongcrushTestEditor::MakePullDownMenu));
			// 메뉴 확장 등록
			LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
		}
		
		// 커스텀 디테일 패널 등록
		{
			FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyEditorModule.RegisterCustomClassLayout(
				AFongcrushEditorTestActor::StaticClass()->GetFName(),
				FOnGetDetailCustomizationInstance::CreateStatic(&FFongcrushTestActorDetail::MakeInstance));
		}
		
		// 커스텀 에셋 등록
		{
			// 에셋 툴 모듈 로드
			IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
			// 에셋 카테고리 등록
			EAssetTypeCategories::Type Category = AssetTools.RegisterAdvancedAssetCategory("Fongcrush", FText::FromString(TEXT("퐁크러시")));
			// 카테고리에 커스텀 에셋 등록
			const TSharedPtr<IAssetTypeActions> Action = MakeShared<FFongcrushDataTypeActions>(Category, UFongcrushTestData::StaticClass());
			AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
			// 등록 해제를 위한 저장
			AssetTypeActions.Emplace(Action);
		}
	}
	
	FFongcrushModuleBase::StartupModule();
}

void FFongcrushTestEditor::ShutdownModule()
{
	// 커스텀 디테일 패널 해제
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(AFongcrushEditorTestActor::StaticClass()->GetFName());
	}
	// 커스텀 에셋 해제
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (TSharedPtr<IAssetTypeActions>  AssetTypeAction : AssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(AssetTypeAction.ToSharedRef());
		}
	}
	AssetTypeActions.Empty();
	
	FFongcrushModuleBase::ShutdownModule();
}

void FFongcrushTestEditor::AddModuleListener()
{
	ModuleListeners.Emplace(MakeShared<FFongcrushMenuListener>());
	ModuleListeners.Emplace(MakeShared<FFongcrushTestTabListener>());
	ModuleListeners.Emplace(MakeShared<FFongcrushEditorModeListener>());
}

void FFongcrushTestEditor::AddMenuExtension(const FName& ExtensionHook,
                                               const FMenuExtensionDelegate& ExtensionDelegate,
                                               const EExtensionHook::Position& Position,
                                               const TSharedPtr<FUICommandList>& CommandList) const
{
	MenuExtender->AddMenuExtension(ExtensionHook, Position, CommandList, ExtensionDelegate);
}

void FFongcrushTestEditor::MakePullDownMenu(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(
		FText::FromString("Fongcrush"),
		FText::FromString("Open Fongcrush Menu"),
		FNewMenuDelegate::CreateRaw(this, &FFongcrushTestEditor::FillPullDownMenu),
		FName("Fongcrush"),
		FName("FongcrushMenu"));
}

void FFongcrushTestEditor::FillPullDownMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection(FName("FongcrushSection"), FText::FromString("Section 1"));
	MenuBuilder.AddMenuSeparator(FName("Section_1"));
	MenuBuilder.EndSection();

	MenuBuilder.BeginSection(FName("FongcrushSection"), FText::FromString("Section 2"));
	MenuBuilder.AddMenuSeparator(FName("Section_2"));
	MenuBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFongcrushTestEditor, FongcrushTestEditor)
