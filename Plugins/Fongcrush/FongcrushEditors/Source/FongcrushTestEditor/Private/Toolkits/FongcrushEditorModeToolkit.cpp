// Fill out your copyright notice in the Description page of Project Settings.


#include "Toolkits/FongcrushEditorModeToolkit.h"

#include "EditorModes/FongcrushEditorMode.h"
#include "Slates/FongcrushEditorModeWidget.h"
#include "EditorModeManager.h"


FFongcrushEditorModeToolkit::FFongcrushEditorModeToolkit()
{
	SAssignNew(EditorModeWidget, SFongcrushEditorModeWidget);
}

FEdMode* FFongcrushEditorModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FFongcrushEditorMode::ModeID);
}

TSharedPtr<SWidget> FFongcrushEditorModeToolkit::GetInlineContent() const
{
	return EditorModeWidget;
}
