// Copyright Epic Games, Inc. All Rights Reserved.

#include "FCSpawner.h"

#include "Components/SpawnerComponent.h"

DEFINE_LOG_CATEGORY(FCSpawner)

#define LOCTEXT_NAMESPACE "FFCSpawnerModule"

void FFCSpawnerModule::StartupModule()
{
	UE_LOG(FCSpawner, Display, TEXT("FCSpawner 모듈 시작"))

	PreChangedHandle = FCoreUObjectDelegates::OnPreObjectPropertyChanged.AddLambda([](UObject* Object, const FEditPropertyChain& PropertyChain)
	{
		TArray<TWeakObjectPtr<USpawnerComponent>> Spawners;

		UObject* TempObject = Object;
		while (TempObject)
		{
			USpawnerComponent::ClassToSpawnersMap.MultiFind(TempObject, Spawners);
			if (Spawners.Num() > 0)
			{
				UE_LOG(FCSpawner, Display, TEXT("======================================================================"))
				UE_LOG(FCSpawner, Display, TEXT("PreChanged: (%p) %s"), Object, *Object->GetFullName())
				break;
			}
			USpawnerComponent::ClassToSpawnersMap.MultiFind(TempObject->GetClass(), Spawners);
			if (Spawners.Num() > 0)
			{
				UE_LOG(FCSpawner, Display, TEXT("======================================================================"))
				UE_LOG(FCSpawner, Display, TEXT("PreChanged: (%p) %s"), Object, *Object->GetFullName())
				break;
			}
			TempObject = TempObject->GetOuter();
		}
		
		for (auto& Spawner : Spawners)
		{
			if (Spawner.IsValid())
			{
				UE_LOG(FCSpawner, Display, TEXT("(%p) %s"), Spawner.Get(), *Spawner->GetFullName())
				Spawner->DestroyPreview();
			}
		}
	});

	ReInstancedHandle = FCoreUObjectDelegates::OnObjectsReinstanced.AddLambda([](const TMap<UObject*, UObject*>& OldToNewMap)
	{
		for (auto& OldToNew : OldToNewMap)
		{
			UObject* OldInst = OldToNew.Key;
			UObject* NewInst = OldToNew.Value;

			TArray<TWeakObjectPtr<USpawnerComponent>> Spawners;

			USpawnerComponent::ClassToSpawnersMap.MultiFind(OldInst, Spawners);
			if (Spawners.Num() > 0 && OldInst != NewInst)
			{
				UE_LOG(FCSpawner, Display, TEXT("======================================================================"))
				UE_LOG(FCSpawner, Display, TEXT("ReInstanced Old: (%p) %s"), OldInst, *OldInst->GetFullName())
				if (NewInst)
				{
					UE_LOG(FCSpawner, Display, TEXT("ReInstanced New: (%p) %s"), NewInst, *NewInst->GetFullName())
					for (auto& Spawner : Spawners)
					{
						USpawnerComponent::ClassToSpawnersMap.AddUnique(NewInst, Spawner);
					}
				}
				USpawnerComponent::ClassToSpawnersMap.Remove(OldInst);
			}
		}
	});

	PostChangedHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.AddLambda([](UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
	{
		TArray<TWeakObjectPtr<USpawnerComponent>> Spawners;

		UObject* TempObject = Object;
		while (TempObject)
		{
			USpawnerComponent::ClassToSpawnersMap.MultiFind(TempObject, Spawners);
			if (Spawners.Num() > 0)
			{
				UE_LOG(FCSpawner, Display, TEXT("======================================================================"))
				UE_LOG(FCSpawner, Display, TEXT("PostChanged: (%p) %s"), Object, *Object->GetFullName())
				break;
			}
			USpawnerComponent::ClassToSpawnersMap.MultiFind(TempObject->GetClass(), Spawners);
			if (Spawners.Num() > 0)
			{
				UE_LOG(FCSpawner, Display, TEXT("======================================================================"))
				UE_LOG(FCSpawner, Display, TEXT("PostChanged: (%p) %s"), Object, *Object->GetFullName())
				break;
			}
			TempObject = TempObject->GetOuter();
		}

		for (auto& Spawner : Spawners)
		{
			if (Spawner.IsValid())
			{
				if (AActor* OwnerActor = Spawner->GetOwner())
				{
					OwnerActor->RerunConstructionScripts();
				}
			}
		}
	});
}

void FFCSpawnerModule::ShutdownModule()
{
	UE_LOG(FCSpawner, Display, TEXT("FCSpawner 모듈 종료"))

	FCoreUObjectDelegates::OnObjectsReinstanced.Remove(ReInstancedHandle);
	FCoreUObjectDelegates::OnObjectPropertyChanged.Remove(PostChangedHandle);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFCSpawnerModule, FCSpawner)
