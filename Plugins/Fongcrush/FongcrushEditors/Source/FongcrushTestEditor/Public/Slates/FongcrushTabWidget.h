// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widgets/SCompoundWidget.h"

class FFongcrushTestTabListener;

/**
 * 
 */
class FONGCRUSHTESTEDITOR_API SFongcrushTabWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFongcrushTabWidget){}
	SLATE_ARGUMENT(TWeakPtr<FFongcrushTestTabListener>, Tool)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
protected:
	TWeakPtr<FFongcrushTestTabListener> Tab;
};
