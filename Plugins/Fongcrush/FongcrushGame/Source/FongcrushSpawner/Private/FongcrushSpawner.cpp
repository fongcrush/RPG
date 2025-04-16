// Copyright Epic Games, Inc. All Rights Reserved.

#include "FongcrushSpawner.h"

#include "ComponentVisualizers.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "Components/SpawnerComponent.h"
#include "Visualizers/SpawnerVisualizer.h"

DEFINE_LOG_CATEGORY(FongcrushSpawner)

#define LOCTEXT_NAMESPACE "FFongcrushSpawnerModule"

void FFongcrushSpawnerModule::StartupModule()
{
	UE_LOG(FongcrushSpawner, Display, TEXT("FongcrushSpawner 모듈 시작"))
#if WITH_EDITOR
	if (GUnrealEd)
	{
		UE_LOG(FongcrushSpawner, Display, TEXT("SpawnerComponentVisualize 등록"))
		FComponentVisualizersModule& ComponentVisualizerModule = FModuleManager::LoadModuleChecked<FComponentVisualizersModule>("ComponentVisualizers");
		ComponentVisualizerModule.RegisterComponentVisualizer(USpawnerComponent::StaticClass()->GetFName(), MakeShared<FSpawnerComponentVisualizer>());
	}
#endif
}

void FFongcrushSpawnerModule::ShutdownModule()
{
#if WITH_EDITOR
	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(USpawnerComponent::StaticClass()->GetFName());
	}
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFongcrushSpawnerModule, FongcrushSpawner)