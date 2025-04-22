// Fill out your copyright notice in the Description page of Project Settings.


#include "Slates/FCExampleTabWidget.h"

void SFCExampleTabWidget::Construct(const FArguments& InArgs)
{
	Tab = InArgs._Tool;
	if (Tab.IsValid())
	{
		// do anything you need from tool object
	}

	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		.VAlign(VAlign_Top)
		.Padding(5)
		[
			SNew(SBorder)
			.BorderBackgroundColor(FColor(192, 192, 192, 255))
			.Padding(15.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("This is a tab example.")))
			]
		]
	];
}
