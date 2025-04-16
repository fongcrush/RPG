// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions/FongcrushDataTypeActions.h"
#include "Objects/FongcrushTestData.h"
#include "EditorReimportHandler.h"

void FFongcrushDataTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	// WeakObjectPtr 배열로 변환
	TArray<TWeakObjectPtr<UFongcrushTestData>> DataImports = GetTypedWeakObjectPtrs<UFongcrushTestData>(InObjects);

	const auto Action = FExecuteAction::CreateSP(this, &FFongcrushDataTypeActions::ExecuteReimport, DataImports);
	const FUIAction UIAction(Action, FCanExecuteAction());
	
	MenuBuilder.AddMenuEntry(
		FText::FromString("Reimport"),
		FText::FromString("Reimport data."),
		FSlateIcon(),
		UIAction
	);
}

void FFongcrushDataTypeActions::ExecuteReimport(TArray<TWeakObjectPtr<UFongcrushTestData>> Objects)
{
    for (const TWeakObjectPtr<UFongcrushTestData>& Object : Objects)
    {
        if (Object.IsValid())
        {
            FReimportManager::Instance()->Reimport(Object.Get(), true);
        }
    }
}
