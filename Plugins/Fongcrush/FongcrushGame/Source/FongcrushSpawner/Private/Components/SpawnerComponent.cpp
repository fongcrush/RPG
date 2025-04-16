// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpawnerComponent.h"


USpawnerComponent::USpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
}

#if WITH_EDITOR
void USpawnerComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	// LOG_CALLINFO_WARNING("SpawnComponent::OnComponentCreated");
	// SpawnPreview();

	
}

void USpawnerComponent::SpawnPreview()
{
	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();
	if (!Owner || !World)
	{
		return;
	}
	
	// PIE / SIE 방지, Only in Preview
	if (GEditor->IsPlayingSessionInEditor())
	{
		return;
	}
	
	// 무한 루프 / 중복 생성 방지
	if (ActorClass && ActorClass != GetClass() && GIsReconstructingBlueprintInstances)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.OverrideLevel = (Owner ? Owner->GetLevel() : nullptr);
		Params.bCreateActorPackage = false;
		Params.OverridePackage = (Owner && !(Params.ObjectFlags & RF_Transient)) ? Owner->GetExternalPackage() : nullptr;
		// Params.OverrideActorGuid = CachedInstanceData ? CachedInstanceData->ChildActorGUID : FGuid();
		// Params.bHideFromSceneOutliner = EditorTreeViewVisualizationMode == EChildActorComponentTreeViewVisualizationMode::Hidden;
		// PreviewActor == World->SpawnActor(ActorClass);
	}
}
#endif

void USpawnerComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(ActorClass, TEXT("%s SpawnActorClass is nullptr"), *GetFullName()))
	{
		DestroyComponent();
	}
}

// Called every frame
void USpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
