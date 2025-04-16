// Fill out your copyright notice in the Description page of Project Settings.


#include "Slates/FongcrushEditorModeWidget.h"
#include "EditorModes/FongcrushEditorMode.h"
#include "EditorModeManager.h"

void SFongcrushEditorModeWidget::Construct(const FArguments& InArgs)
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
				.Text(FText::FromString(TEXT("Mode Fongcrush")))
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
					.OnClicked(this, &SFongcrushEditorModeWidget::OnAddPoint)
					.IsEnabled(this, &SFongcrushEditorModeWidget::CanAddOrRemovePoint)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0, 0, 2, 0)
				[
					SNew(SButton)
					.Text(FText::FromString("Remove"))
					.OnClicked(this, &SFongcrushEditorModeWidget::OnRemovePoint)
					.IsEnabled(this, &SFongcrushEditorModeWidget::CanAddOrRemovePoint)
				]
			]
		]
	];

}

FFongcrushEditorMode* SFongcrushEditorModeWidget::GetEditorMode() const
{
	return StaticCast<FFongcrushEditorMode*>(GLevelEditorModeTools().GetActiveMode(FFongcrushEditorMode::ModeID));
}

FReply SFongcrushEditorModeWidget::OnAddPoint() const
{
	GetEditorMode()->AddPoint();
	return FReply::Handled();
}

FReply SFongcrushEditorModeWidget::OnRemovePoint() const
{	
	GetEditorMode()->RemovePoint();
	return FReply::Handled();
}

bool SFongcrushEditorModeWidget::CanAddOrRemovePoint() const
{
	return GetEditorMode() ? GetEditorMode()->HasValidSelection() : false;
}
