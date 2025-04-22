// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class SFCExampleEdModeWidget;

/**
 * Toolkit - 도구 모음
 * FModeToolkit - 모드별 도구 패널(Details, 버튼, UI)을 생성하는 클래스
 * FCEditorModeWidget을 생성
 */
class FCEXAMPLEEDITOR_API FFCExampleEdModeToolkit : public FModeToolkit
{
public:
	/** SAssignNew(EditorModeWidget, SFCExampleEdModeWidget) */
	FFCExampleEdModeToolkit();

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override { return FName("FCMode"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("BuilderModeToolkit", "DisplayName", "Builder"); }
	virtual FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<SWidget> GetInlineContent() const override;

private:
	TSharedPtr<SFCExampleEdModeWidget> EditorModeWidget;
};
