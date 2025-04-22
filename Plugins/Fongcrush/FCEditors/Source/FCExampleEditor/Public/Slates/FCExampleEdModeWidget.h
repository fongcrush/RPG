// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class FFCExampleEdMode;

/**
 * 
 */
class FCEXAMPLEEDITOR_API SFCExampleEdModeWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFCExampleEdModeWidget)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	FFCExampleEdMode* GetEditorMode() const;
	
	FReply OnAddPoint() const;
	FReply OnRemovePoint() const;
	bool CanAddOrRemovePoint() const;
};
