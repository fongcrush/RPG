﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "DetailCustomizations/FCExampleActorDetail.h"

#include "DetailCategoryBuilder.h"
#include "Objects/FCExampleEditorActor.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "EditorStyleSet.h"

void FFCExampleActorDetail::CustomizeDetails(IDetailLayoutBuilder& DetailLayoutBuilder)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailLayoutBuilder.GetObjectsBeingCustomized(Objects);
	if (Objects.Num() != 1)
	{
		// skip customization if select more than one objects
		return;
	}
	AFCExampleEditorActor* TestActor = Cast<AFCExampleEditorActor>(Objects[0].Get());

	// hide original property
	DetailLayoutBuilder.HideProperty(DetailLayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AFCExampleEditorActor, bOption1)));
	DetailLayoutBuilder.HideProperty(DetailLayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AFCExampleEditorActor, bOption2)));

	// add custom widget to "Options" category
	IDetailCategoryBuilder& OptionsCategory =
		DetailLayoutBuilder.EditCategory(
			"Options",
			FText::FromString(""),
			ECategoryPriority::Important);

	OptionsCategory
		.AddCustomRow(FText::FromString("Options"))
		.WholeRowContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SCheckBox)
				.Style(FAppStyle::Get(), "RadioButton")
				.IsChecked(this, &FFCExampleActorDetail::IsModeRadioChecked, TestActor, 1)
				.OnCheckStateChanged(this, &FFCExampleActorDetail::OnModeRadioChanged, TestActor, 1)
				[
					SNew(STextBlock).Text(FText::FromString("Option 1"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(10.f, 0.f, 0.f, 0.f)
			.VAlign(VAlign_Center)
			[
				SNew(SCheckBox)
				.Style(FAppStyle::Get(), "RadioButton")
				.IsChecked(this, &FFCExampleActorDetail::IsModeRadioChecked, TestActor, 2)
				.OnCheckStateChanged(this, &FFCExampleActorDetail::OnModeRadioChanged, TestActor, 2)
				[
					SNew(STextBlock).Text(FText::FromString("Option 2"))
				]
			]
		];
}

ECheckBoxState FFCExampleActorDetail::IsModeRadioChecked(AFCExampleEditorActor* InActor, int OptionIndex) const
{
	bool bFlag = false;
	if (InActor)
	{
		if (OptionIndex == 1)
			bFlag = InActor->bOption1;
		else if (OptionIndex == 2)
			bFlag = InActor->bOption2;
	}
	return bFlag ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void FFCExampleActorDetail::OnModeRadioChanged(ECheckBoxState CheckType, AFCExampleEditorActor* InActor, int OptionIndex)
{
	bool bFlag = (CheckType == ECheckBoxState::Checked);
	if (InActor)
	{
		InActor->Modify();
		if (bFlag)
		{
			// clear all options first
			InActor->bOption1 = false;
			InActor->bOption2 = false;
		}
		if (OptionIndex == 1)
			InActor->bOption1 = bFlag;
		else if (OptionIndex == 2)
			InActor->bOption2 = bFlag;
	}
}
