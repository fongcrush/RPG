// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ModuleListeners/FCExampleModuleBase.h"

class FWorkspaceItem;
class IAssetTypeActions;

class FFCExampleEditor : public FFCExampleModuleBase
{
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void AddModuleListener() override;
	void AddMenuExtension(const FName& ExtensionHook,
	                      const FMenuExtensionDelegate& ExtensionDelegate,
	                      const EExtensionHook::Position& Position = EExtensionHook::Position::Before,
	                      const TSharedPtr<FUICommandList>& CommandList = nullptr) const;


	static FORCEINLINE FFCExampleEditor& Get() { return FModuleManager::LoadModuleChecked<FFCExampleEditor>("FCExampleEditor"); }
	static FORCEINLINE bool IsLoaded() { return FModuleManager::Get().IsModuleLoaded("FCExampleEditor"); }

	FORCEINLINE TSharedRef<FWorkspaceItem> GetMenuRoot() const { return MenuRoot; }

protected:
	/** 하단으로 확장되는 메뉴(FC) 버튼을 만듭니다. */
	void MakePullDownMenu(FMenuBarBuilder& MenuBuilder);
	/**  */
	void FillPullDownMenu(FMenuBuilder& MenuBuilder);

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	static TSharedRef<FWorkspaceItem> MenuRoot;
	
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;

	TArray<TSharedPtr<IAssetTypeActions>> AssetTypeActions;
	
};
