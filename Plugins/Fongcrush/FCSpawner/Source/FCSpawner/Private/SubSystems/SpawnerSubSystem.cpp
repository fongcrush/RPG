// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/SpawnerSubSystem.h"

#include "EngineUtils.h"
#include "FCSpawner.h"
#include "Actors/SpawnerPreviewActor.h"
#include "Components/SpawnerComponent.h"


void USpawnerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(FCSpawner, Display, TEXT("SpawnerSubSystem 초기화"));

	for (const auto& Actor : TActorRange<AActor>(GetWorld()))
	{
		if (ASpawnerPreviewActor* PreviewActor = Cast<ASpawnerPreviewActor>(Actor))
		{
			UE_LOG(FCSpawner, Display, TEXT("PreviewActor: %s"), *PreviewActor->GetName());
			PreviewActors.Emplace(PreviewActor);
		}

		for (const auto& Component : Actor->GetComponents())
		{
			if (USpawnerComponent* Spawner = Cast<USpawnerComponent>(Component))
			{
				UE_LOG(FCSpawner, Display, TEXT("Spawner: %s"), *Spawner->GetName());
				SpawnerComps.Emplace(Spawner);
			}
		}
	}
}