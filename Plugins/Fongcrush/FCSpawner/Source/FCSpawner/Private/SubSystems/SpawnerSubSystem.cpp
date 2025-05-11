// Fill out your copyright notice in the Description page of Project Settings.
#if WITH_EDITOR

#include "SubSystems/SpawnerSubSystem.h"

#include "FCSpawner.h"
#include "Components/SpawnerComponent.h"


void USpawnerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(FCSpawner, Display, TEXT("SpawnerSubSystem 초기화"))

	ObjectsReplacedHandle = FCoreUObjectDelegates::OnObjectsReplaced.AddUObject(this, &USpawnerSubSystem::OnObjectsReplaced);

	// FCoreUObjectDelegates::OnObjectsReplaced.AddLambda([](const TMap<UObject*, UObject*>& OldToNewMap)
	// {
	// 	if (OldToNewMap.IsEmpty()) return;
	// 	
	// 	UE_LOG(FCSpawner, Display, TEXT("Replaced ===================================================================================================================="))
	// 	for (auto& Pair : OldToNewMap)
	// 	{
	// 		UE_LOG(FCSpawner, Display, TEXT("Old: %s"), Pair.Key ? *Pair.Key->GetFullName() : *FString("None"))
	// 		UE_LOG(FCSpawner, Display, TEXT("New: %s"), Pair.Value ? *Pair.Value->GetFullName() : *FString("None"))
	// 		UE_LOG(FCSpawner, Display, TEXT("-----------------------------------------------------------------------------------------------------------------------------"))
	// 	}
	// 	UE_LOG(FCSpawner, Display, TEXT(" "))
	// });
	//
	// FCoreUObjectDelegates::OnObjectsReinstanced.AddLambda([](const TMap<UObject*, UObject*>& OldToNewMap)
	// {
	// 	if (OldToNewMap.IsEmpty()) return;
	// 	
	// 	UE_LOG(FCSpawner, Display, TEXT("Reinstanced ================================================================================================================="))
	// 	for (auto& Pair : OldToNewMap)
	// 	{
	// 		UE_LOG(FCSpawner, Display, TEXT("Old: %s"), Pair.Key ? *Pair.Key->GetFullName() : *FString("None"))
	// 		UE_LOG(FCSpawner, Display, TEXT("New: %s"), Pair.Value ? *Pair.Value->GetFullName() : *FString("None"))
	// 		UE_LOG(FCSpawner, Display, TEXT("-----------------------------------------------------------------------------------------------------------------------------"))
	// 	}
	// 	UE_LOG(FCSpawner, Display, TEXT(" "))
	// });
	
}

void USpawnerSubSystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(FCSpawner, Display, TEXT("SpawnerSubSystem 해제"))

	FCoreUObjectDelegates::OnObjectsReplaced.Remove(ObjectsReplacedHandle);
	// FCoreUObjectDelegates::OnObjectsReplaced.RemoveAll(this);
	// FCoreUObjectDelegates::OnObjectsReinstanced.RemoveAll(this);
}

void USpawnerSubSystem::OnObjectsReplaced(const TMap<UObject*, UObject*>& OldToNewMap)
{
	if (OldToNewMap.IsEmpty()) return;
	
	for (auto& Pair : OldToNewMap)
	{
		USpawnerComponent** SpawnerPtr = TemplateToSpawnerMap.Find(Pair.Key);
		if (SpawnerPtr && *SpawnerPtr && Pair.Value)
		{			
			USpawnerComponent* Spawner = *SpawnerPtr;
			// if (!Spawner->IsTemplate())
			// {
			// 	Spawner->ActorTemplate = Cast<AActor>(Pair.Value);
			// }
			// if (Pair.Value->GetWorld()->WorldType == EWorldType::Editor)
			// {
			// 	// Spawner->UpdatePreview();
			// }
			
			TemplateToSpawnerMap.Add(Pair.Value, Spawner);
			TemplateToSpawnerMap.Remove(Pair.Key);
		}
	}
}

int32 USpawnerSubSystem::GetSelectedSpawners(TArray<UObject*>& OutSelected) const
{
	OutSelected.Reserve(Spawners.Num());
	for (auto& Spawner : Spawners)
	{
		if (Spawner.IsValid() && Spawner->IsSelectedInEdMode())
		{
			OutSelected.Emplace(Spawner.Get());
		}
	}
	return OutSelected.Num();
}

FString USpawnerSubSystem::GetWorldTypeString(UObject* const& Object)
{
	if (IsValid(Object))
	{
		if (UWorld* World = Object->GetWorld())
		{
			FString WorldName = World->GetFullName();
			switch (World->WorldType)
			{
			case EWorldType::None: return "None(" + WorldName + ")";
			case EWorldType::Game: return "Game(" + WorldName + ")";
			case EWorldType::Editor: return "Editor(" + WorldName + ")";
			case EWorldType::PIE: return "PIE(" + WorldName + ")";
			case EWorldType::EditorPreview: return "EditorPreview(" + WorldName + ")";
			case EWorldType::GamePreview: return "GamePreview(" + WorldName + ")";
			case EWorldType::GameRPC: return "GameRPC(" + WorldName + ")";
			case EWorldType::Inactive: return "Inactive(" + WorldName + ")";
			default: break;
			}
		}
	}
	return "InValid";
}

#endif
