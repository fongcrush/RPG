// Fill out your copyright notice in the Description page of Project Settings.
#if WITH_EDITOR

#include "SubSystems/SpawnerSubSystem.h"

#include "Components/SpawnerComponent.h"

UEditorWorldSpawnerSubSystem::UEditorWorldSpawnerSubSystem()
{
}

int32 UEditorWorldSpawnerSubSystem::GetSelectedSpawners(TArray<UObject*>& OutSelected) const
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

#endif
