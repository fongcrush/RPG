

#pragma once

#include "ModuleListeners/FongcrushModuleBase.h"

class FONGCRUSHTESTEDITOR_API FFongcrushMenuListener : public IModuleListener, public TSharedFromThis<FFongcrushMenuListener>
{
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual ~FFongcrushMenuListener() {};
	virtual void OnStartupModule() override;
	virtual void OnShutdownModule() override;
	/** 메뉴 항목 생성 */
	void MakeMenuEntry(FMenuBuilder& MenuBuilder);
	/** 서브 메뉴 항목 생성 */
	void MakeSubMenu(FMenuBuilder& MenuBuilder);

protected:
	/** 명령 맵핑 */
	void BindCommands();
	/** 명령 1*/
	void MenuCommand01();

	FString GetSelectedActorNames();
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	TSharedPtr<FUICommandList> UICommandList;
	TSharedPtr<SEditableText> TextWidget;
	
};