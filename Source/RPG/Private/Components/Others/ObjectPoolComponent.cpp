// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Others/ObjectPoolComponent.h"


UObjectPoolComponent::UObjectPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UObjectPoolComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	PoolMap.Remove(nullptr);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for(auto& Pool : PoolMap)
	{
		PoolWeakPtrMap.Add(Pool.Key, MakeShared<FPool>(Pool.Value));
	}
}

// Called when the game starts
void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for(TTuple<TSubclassOf<UObject>, FPool>& PoolTuple : PoolMap)
	{
		const TSubclassOf<UObject>& ObjectClass = PoolTuple.Key;
		FPool& Pool = PoolTuple.Value;
		
		if(ObjectClass && Pool.Num > 0)
		{
			// 엑터 기반인 경우
			if(ObjectClass->IsChildOf(AActor::StaticClass()))
			{
				for(int i = 0; i < Pool.Num; i++)
				{
					// 엑터 생성, 비활성화 후 풀에 추가
					AActor* NewActor = GetWorld()->SpawnActor<AActor>(ObjectClass, SpawnParams);
					NewActor->SetFolderPath(FName(*FString::Printf(TEXT("ObjectPool/%s"), *ObjectClass->GetName())));
					NewActor->SetActorHiddenInGame(true);
					NewActor->SetActorEnableCollision(false);
					NewActor->SetActorTickEnabled(false);
					Pool.Stack.Push(NewActor);
				}
			}
			else
			{
				for(int i = 0; i < Pool.Num; i++)
				{
					// 오브젝트 생성 후 풀에 추가
					UObject* Object = NewObject<UObject>(GetWorld(), ObjectClass);
					Pool.Stack.Push(Object);
				}
			}
		}
	}
}
