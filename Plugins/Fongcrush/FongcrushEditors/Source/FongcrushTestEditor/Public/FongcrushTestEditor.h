// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ModuleListeners/FongcrushModuleBase.h"

class FWorkspaceItem;
class IAssetTypeActions;

class FFongcrushTestEditor : public FFongcrushModuleBase
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


	static FORCEINLINE FFongcrushTestEditor& Get() { return FModuleManager::LoadModuleChecked<FFongcrushTestEditor>("FongcrushTestEditor"); }
	static FORCEINLINE bool IsLoaded() { return FModuleManager::Get().IsModuleLoaded("FongcrushTestEditor"); }

	FORCEINLINE TSharedRef<FWorkspaceItem> GetMenuRoot() const { return MenuRoot; }

protected:
	/** 하단으로 확장되는 메뉴(Fongcrush) 버튼을 만듭니다. */
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
