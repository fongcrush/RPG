// Fill out your copyright notice in the Description page of Project Settings.
#if WITH_EDITOR

#include "SubSystems/SpawnerSubSystem.h"

#include "FCSpawner.h"
#include "Components/SpawnerComponent.h"


void USpawnerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(FCSpawner, Display, TEXT("SpawnerSubSystem 초기화"))
}

void USpawnerSubSystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(FCSpawner, Display, TEXT("SpawnerSubSystem 해제"))
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
