// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class FFongcrushEditorMode;

/**
 * 
 */
class FONGCRUSHTESTEDITOR_API SFongcrushEditorModeWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFongcrushEditorModeWidget)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	FFongcrushEditorMode* GetEditorMode() const;
	
	FReply OnAddPoint() const;
	FReply OnRemovePoint() const;
	bool CanAddOrRemovePoint() const;
};
