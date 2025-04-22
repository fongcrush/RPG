// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widgets/SCompoundWidget.h"

class FFCExampleTabListener;

/**
 * 
 */
class FCEXAMPLEEDITOR_API SFCExampleTabWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFCExampleTabWidget){}
	SLATE_ARGUMENT(TWeakPtr<FFCExampleTabListener>, Tool)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
protected:
	TWeakPtr<FFCExampleTabListener> Tab;
};
