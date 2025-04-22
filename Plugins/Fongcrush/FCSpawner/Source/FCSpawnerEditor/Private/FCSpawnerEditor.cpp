#include "FCSpawnerEditor.h"

#include "UnrealEdGlobals.h"
#include "Visualizers/FCSpawnerVisualizer.h"
#include "ComponentVisualizers.h"
#include "EditorModeRegistry.h"
#include "Selection.h"
#include "Editor/UnrealEdEngine.h"
#include "EditorModes/FCSpawnerEdMode.h"

#define LOCTEXT_NAMESPACE "FCSpawnerEditorModule"

DEFINE_LOG_CATEGORY(FCSpawnerEditor)

void FFCSpawnerEditorModule::StartupModule()
{
	UE_LOG(FCSpawnerEditor, Display, TEXT("SpawnerEditor 모듈 시작"))
	FModuleManager::LoadModuleChecked<FComponentVisualizersModule>("UnrealEd");
	if (GUnrealEd)
	{
		UE_LOG(FCSpawnerEditor, Display, TEXT("SpawnerComponentVisualize 등록"))
		// SpawnerComponentVisualize 등록
		FComponentVisualizersModule& ComponentVisualizerModule = FModuleManager::LoadModuleChecked<FComponentVisualizersModule>("ComponentVisualizers");
		ComponentVisualizerModule.RegisterComponentVisualizer(USpawnerComponent::StaticClass()->GetFName(), MakeShared<FFCSpawnerComponentVisualizer>());
	}

	// 스포너 에디터 모드 등록
	FEditorModeRegistry::Get().RegisterMode<FFCSpawnerEdMode>(
		FFCSpawnerEdMode::ModeID,
		FText::FromString(TEXT("스포너")),
		FSlateIcon(),
		true
	);
}

void FFCSpawnerEditorModule::ShutdownModule()
{
	if (GUnrealEd)
	{
		TSharedPtr<FComponentVisualizer> SpawnerVisualizer = GUnrealEd->FindComponentVisualizer(USpawnerComponent::StaticClass()->GetFName());
		
		// OnSelected 해제
		USelection::SelectionChangedEvent.RemoveAll(SpawnerVisualizer.Get());
		USelection::SelectObjectEvent.RemoveAll(SpawnerVisualizer.Get())
		;
		// SpawnerComponentVisualize 해제
		GUnrealEd->UnregisterComponentVisualizer(USpawnerComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFCSpawnerEditorModule, FCSpawnerEditor)
