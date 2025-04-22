// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions/FCExampleDataTypeActions.h"
#include "Objects/FCExampleData.h"
#include "EditorReimportHandler.h"

void FFCExampleDataTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	// WeakObjectPtr 배열로 변환
	TArray<TWeakObjectPtr<UFCExampleData>> DataImports = GetTypedWeakObjectPtrs<UFCExampleData>(InObjects);

	const auto Action = FExecuteAction::CreateSP(this, &FFCExampleDataTypeActions::ExecuteReimport, DataImports);
	const FUIAction UIAction(Action, FCanExecuteAction());
	
	MenuBuilder.AddMenuEntry(
		FText::FromString("Reimport"),
		FText::FromString("Reimport data."),
		FSlateIcon(),
		UIAction
	);
}

void FFCExampleDataTypeActions::ExecuteReimport(TArray<TWeakObjectPtr<UFCExampleData>> Objects)
{
    for (const TWeakObjectPtr<UFCExampleData>& Object : Objects)
    {
        if (Object.IsValid())
        {
            FReimportManager::Instance()->Reimport(Object.Get(), true);
        }
    }
}
