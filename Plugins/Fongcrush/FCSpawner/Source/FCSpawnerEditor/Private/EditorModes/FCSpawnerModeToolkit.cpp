// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorModes/FCSpawnerModeToolkit.h"

#include "EditorModeManager.h"
#include "FCSpawnerEditor.h"
#include "Components/SpawnerComponent.h"
#include "EditorModes/FCSpawnerEdMode.h"
#include "SubSystems/SpawnerSubSystem.h"


void FFCSpawnerModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs Args;
	Args.bAllowSearch = false;
	Args.bHideSelectionTip = false;
	Args.bShowScrollBar = false;
	Args.bShowOptions = false;

	SpawnerDetailsView = PropertyEditorModule.CreateDetailView(Args);

	// Spawner 카테고리로 필터링. Transform은 IDetailCustomization 으로 특수 변형되었으므로 필터링 불가. SetObject 이전에 설정해야 정상 작동
	SpawnerDetailsView->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateLambda([](const FPropertyAndParent& PropertyAndParent)
	{
		const FProperty& Property = PropertyAndParent.Property;
		const FString& PropertyName = Property.GetMetaData(TEXT("Category"));
		// Spawner 카테고리만 통과
		return PropertyName.Equals(TEXT("Spawner"));
	}));
}

TSharedPtr<SWidget> FFCSpawnerModeToolkit::GetInlineContent() const
{
	TSharedRef<SWidget> InheritContent = FModeToolkit::GetInlineContent().ToSharedRef();

	FFCSpawnerEdMode* SpawnerEdMode = StaticCast<FFCSpawnerEdMode*>(GetEditorMode());
	if (SpawnerEdMode == nullptr)
	{
		return InheritContent;
	}

	SpawnerDetailsView->RemoveInvalidObjects();

	return SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			[
				SNew(SHorizontalBox)

				// Spawner 리셋 버튼 삽입
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(5, 0, 0, 5)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Reset")))
					.ToolTipText(FText::FromString(TEXT("선택된 Spawner의 상태를 초기값으로 리셋")))
					.OnClicked_Lambda([this]()
					{
						FScopedTransaction Transaction(FText::FromString(TEXT("선택된 모든 Spawner 상태 초기화")));
						for (const TWeakObjectPtr<UObject>& SpawnerWeak : SpawnerDetailsView->GetSelectedObjects())
						{
							if (!SpawnerWeak.IsValid()) continue;

							if (USpawnerComponent* Spawner = Cast<USpawnerComponent>(SpawnerWeak.Get()))
							{
								Spawner->Modify();
								Spawner->Reset();
							}
						}
						return FReply::Handled();
					})
				]

				// 선택 해제 버튼 삽입
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(5, 0, 0, 5)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Deselect")))
					.ToolTipText(FText::FromString(TEXT("선택된 모든 Spawner를 선택 해제")))
					.OnClicked_Lambda([SpawnerEdMode]()
					{
						// 상태 저장
						SpawnerEdMode->DeSelectAll();
						return FReply::Handled();
					})
				]

			]

			// 구분선 삽입
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SSeparator)
			]

			// 디테일 뷰
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SpawnerDetailsView.ToSharedRef()
			];
}

FEdMode* FFCSpawnerModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FFCSpawnerEdMode::ModeID);
}
