// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorModes/FCSpawnerModeToolkit.h"

#include "EditorModeManager.h"
#include "EditorModes/FCSpawnerEdMode.h"


void FFCSpawnerModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
	
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	FDetailsViewArgs Args;
	Args.bAllowSearch = false;
	Args.bHideSelectionTip = false;
	Args.bShowScrollBar = false;
	Args.bShowOptions = false;
	
	DetailsView = PropertyEditorModule.CreateDetailView(Args);
	
	// Spawner 카테고리로 필터링. Transform은 IDetailCustomization 으로 특수 변형되었으므로 필터링 불가. SetObject 이전에 설정해야 정상 작동
	DetailsView->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateLambda([](const FPropertyAndParent& PropertyAndParent)
	{
		const FProperty& Property = PropertyAndParent.Property;
		const FString& PropertyName = Property.GetMetaData(TEXT("Category"));
		// Spawner 카테고리만 통과
		if (PropertyName.Equals(TEXT("Spawner")))
		{
			return true;
		}
		return false;
	}));
	
	FFCSpawnerEdMode* SpawnerEdMode = StaticCast<FFCSpawnerEdMode*>(GLevelEditorModeTools().GetActiveMode(FFCSpawnerEdMode::ModeID));
	DetailsView->SetObjects(SpawnerEdMode->GetSelectedSpawners().Array());
}

FEdMode* FFCSpawnerModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FFCSpawnerEdMode::ModeID);
}
