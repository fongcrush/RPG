// Fill out your copyright notice in the Description page of Project Settings.


#include "Toolkits/FCExampleEdModeToolkit.h"

#include "EditorModes/FCExampleEdMode.h"
#include "Slates/FCExampleEdModeWidget.h"
#include "EditorModeManager.h"


FFCExampleEdModeToolkit::FFCExampleEdModeToolkit()
{
	SAssignNew(EditorModeWidget, SFCExampleEdModeWidget);
}

FEdMode* FFCExampleEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FFCExampleEdMode::ModeID);
}

TSharedPtr<SWidget> FFCExampleEdModeToolkit::GetInlineContent() const
{
	return EditorModeWidget;
}
