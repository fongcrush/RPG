// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/SpawnerPreviewActor.h"

#include "FCSpawner.h"
#include "Components/SpawnerComponent.h"
#include "SubSystems/SpawnerSubSystem.h"

// Sets default values
ASpawnerPreviewActor::ASpawnerPreviewActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bIsEditorOnlyActor = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	RootComponent->SetMobility(EComponentMobility::Movable);
}

void ASpawnerPreviewActor::PostInitProperties()
{
	Super::PostInitProperties();
	if (UWorld* World = GetWorld())
	{
		// 스포너 시스템의 참조 등록
		World->GetSubsystem<USpawnerSubSystem>()->PreviewActors.Emplace(this);
	}
}

void ASpawnerPreviewActor::Destroyed()
{
	Super::Destroyed();
	UE_LOG(FCSpawner, Display, TEXT("%s Destroyed"), *GetName());	
	if (UWorld* World = GetWorld())
	{
		// 스포너 시스템의 참조 제거
		World->GetSubsystem<USpawnerSubSystem>()->PreviewActors.Remove(this);
	}
}

void ASpawnerPreviewActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (OwnerSpawner)
	{
		SetActorTransform(OwnerSpawner->GetComponentTransform());
		
		// UE_LOG(FCSpawner, Display, TEXT("PreviewActor Location: %s"), *GetActorLocation().ToString());
	}
}

bool ASpawnerPreviewActor::ShouldTickIfViewportsOnly() const
{
	return GetWorld()->IsPreviewWorld();
}
