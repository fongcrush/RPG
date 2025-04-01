// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Spawners/SpawnComponent.h"

#include "RPG/RPG.h"


USpawnComponent::USpawnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	
	ArrowColor = FColor::Blue;
	ArrowSize = 0.7f;
	ArrowLength = 60.f;
}

void USpawnComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(ActorClass, TEXT("%s SpawnActorClass is nullptr"), *GetFullName()))
	{
		DestroyComponent();
	}
}

// Called every frame
void USpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentTime > SpawnTime)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = bAttached? GetOwner() : nullptr;
		SpawnParameters.bNoFail = true;
		
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, GetComponentLocation(), GetComponentRotation(), SpawnParameters);
		if (bAttached) SpawnedActor->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		if (CountPolicy == ESpawnCountPolicy::Once)
		{
			DestroyComponent();
		}
		else if (CountPolicy == ESpawnCountPolicy::Constant)
		{
			CurrentTime = 0.0f;
			SpawnCount--;
			if (SpawnCount <= 0)
			{
				DestroyComponent();
			}
		}
		else
		{
			CurrentTime = 0.0f;
		}
	}
	CurrentTime += DeltaTime;
}
