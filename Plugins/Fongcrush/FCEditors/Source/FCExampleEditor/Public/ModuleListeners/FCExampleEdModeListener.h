// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FCExampleTapListenerBase.h"

/**
 * 
 */
class FCEXAMPLEEDITOR_API FFCExampleEdModeListener : public FFCExampleTapListenerBase
{
public:
	virtual void OnStartupModule() override;
	virtual void OnShutdownModule() override;

private:
	void RegisterStyleSet();
	void RegisterEditorMode();
	
	void UnregisterStyleSet();
	void UnregisterEditorMode();

	// FSlateStyleSet - Slate 스타일 설정용 클래스 
	static TSharedPtr<FSlateStyleSet> StyleSet;
};
