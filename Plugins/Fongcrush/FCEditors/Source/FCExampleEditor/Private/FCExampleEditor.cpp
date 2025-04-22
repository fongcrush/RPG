// Copyright Epic Games, Inc. All Rights Reserved.

#include "FCExampleEditor.h"

#include "Objects/FCExampleData.h"
#include "Objects/FCExampleEditorActor.h"
#include "AssetTypeActions/FCExampleDataTypeActions.h"
#include "ModuleListeners/FCExampleMenuListener.h"
#include "ModuleListeners/FCExampleTabListener.h"
#include "ModuleListeners/FCExampleEdModeListener.h"
#include "DetailCustomizations/FCExampleActorDetail.h"
#include "LevelEditor.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FFCExampleEditor"

TSharedRef<FWorkspaceItem> FFCExampleEditor::MenuRoot = FWorkspaceItem::NewGroup(FText::FromString("Menu Root"));

void FFCExampleEditor::StartupModule()
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
				FMenuBarExtensionDelegate::CreateRaw(this, &FFCExampleEditor::MakePullDownMenu));
			// 메뉴 확장 등록
			LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
		}
		
		// 커스텀 디테일 패널 등록
		{
			FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyEditorModule.RegisterCustomClassLayout(
				AFCExampleEditorActor::StaticClass()->GetFName(),
				FOnGetDetailCustomizationInstance::CreateStatic(&FFCExampleActorDetail::MakeInstance));
		}
		
		// 커스텀 에셋 등록
		{
			// 에셋 툴 모듈 로드
			IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
			// 에셋 카테고리 등록
			EAssetTypeCategories::Type Category = AssetTools.RegisterAdvancedAssetCategory("FC", FText::FromString(TEXT("퐁크러시")));
			// 카테고리에 커스텀 에셋 등록
			const TSharedPtr<IAssetTypeActions> Action = MakeShared<FFCExampleDataTypeActions>(Category, UFCExampleData::StaticClass());
			AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
			// 등록 해제를 위한 저장
			AssetTypeActions.Emplace(Action);
		}
	}
	
	FFCExampleModuleBase::StartupModule();
}

void FFCExampleEditor::ShutdownModule()
{
	// 커스텀 디테일 패널 해제
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(AFCExampleEditorActor::StaticClass()->GetFName());
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
	
	FFCExampleModuleBase::ShutdownModule();
}

void FFCExampleEditor::AddModuleListener()
{
	ModuleListeners.Emplace(MakeShared<FFCExampleMenuListener>());
	ModuleListeners.Emplace(MakeShared<FFCExampleTabListener>());
	ModuleListeners.Emplace(MakeShared<FFCExampleEdModeListener>());
}

void FFCExampleEditor::AddMenuExtension(const FName& ExtensionHook,
                                               const FMenuExtensionDelegate& ExtensionDelegate,
                                               const EExtensionHook::Position& Position,
                                               const TSharedPtr<FUICommandList>& CommandList) const
{
	MenuExtender->AddMenuExtension(ExtensionHook, Position, CommandList, ExtensionDelegate);
}

void FFCExampleEditor::MakePullDownMenu(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(
		FText::FromString("FC"),
		FText::FromString("Open FC Menu"),
		FNewMenuDelegate::CreateRaw(this, &FFCExampleEditor::FillPullDownMenu),
		FName("FC"),
		FName("FCMenu"));
}

void FFCExampleEditor::FillPullDownMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection(FName("FCSection"), FText::FromString("Section 1"));
	MenuBuilder.AddMenuSeparator(FName("Section_1"));
	MenuBuilder.EndSection();

	MenuBuilder.BeginSection(FName("FCSection"), FText::FromString("Section 2"));
	MenuBuilder.AddMenuSeparator(FName("Section_2"));
	MenuBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFCExampleEditor, FCExampleEditor)
