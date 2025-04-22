// Fill out your copyright notice in the Description page of Project Settings.


#include "Slates/FCExampleEdModeWidget.h"
#include "EditorModes/FCExampleEdMode.h"
#include "EditorModeManager.h"

void SFCExampleEdModeWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		.VAlign(VAlign_Top)
		.Padding(5.f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.f, 5.f, 0.f, 0.f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Mode FC")))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.f, 5.f, 0.f, 0.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(2, 0, 0, 0)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
					.Text(FText::FromString("Add"))
					.OnClicked(this, &SFCExampleEdModeWidget::OnAddPoint)
					.IsEnabled(this, &SFCExampleEdModeWidget::CanAddOrRemovePoint)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0, 0, 2, 0)
				[
					SNew(SButton)
					.Text(FText::FromString("Remove"))
					.OnClicked(this, &SFCExampleEdModeWidget::OnRemovePoint)
					.IsEnabled(this, &SFCExampleEdModeWidget::CanAddOrRemovePoint)
				]
			]
		]
	];

}

FReply SFCExampleEdModeWidget::OnAddPoint() const
{
	GetEditorMode()->AddPoint();
	return FReply::Handled();
}

FReply SFCExampleEdModeWidget::OnRemovePoint() const
{	
	GetEditorMode()->RemovePoint();
	return FReply::Handled();
}

bool SFCExampleEdModeWidget::CanAddOrRemovePoint() const
{
	return GetEditorMode() ? GetEditorMode()->HasValidSelection() : false;
}

FFCExampleEdMode* SFCExampleEdModeWidget::GetEditorMode() const
{
	return StaticCast<FFCExampleEdMode*>(GLevelEditorModeTools().GetActiveMode(FFCExampleEdMode::ModeID));
}