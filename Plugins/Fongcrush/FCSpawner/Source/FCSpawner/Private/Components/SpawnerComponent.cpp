// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpawnerComponent.h"

#include "FCSpawner.h"

#if WITH_EDITOR
#include "Actors/SpawnerPreviewActor.h"
#include "SubSystems/SpawnerSubSystem.h"
#endif


USpawnerComponent::USpawnerComponent(): SpawnTime(0), CurrentTime(0), CountPolicy(ESpawnCountPolicy::Once), SpawnCount(0), bAttached(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
}

#if WITH_EDITOR

void USpawnerComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	
	UWorld* World = GetWorld();
	if (World && World->WorldType == EWorldType::Editor)
	{
		if (PropertyName == GET_MEMBER_NAME_CHECKED(USpawnerComponent, ActorClass))
		{
			InitPreview();
		}
	}
}

void USpawnerComponent::OnRegister()
{
	Super::OnRegister();

	UWorld* World = GetWorld();
	if (World && World->WorldType == EWorldType::Editor)
	{
		if (USpawnerSubSystem* SubSystem = World->GetSubsystem<USpawnerSubSystem>())
		{
			SubSystem->SpawnerComps.Emplace(this);
		}

		InitPreview();
	}
}

void USpawnerComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	UE_LOG(FCSpawner, Display, TEXT("%s OnComponentDestroyed"), *GetName());

	UWorld* World = GetWorld();
	if (World && World->WorldType == EWorldType::Editor)
	{
		if (USpawnerSubSystem* SubSystem = World->GetSubsystem<USpawnerSubSystem>())
		{
			World->GetSubsystem<USpawnerSubSystem>()->SpawnerComps.Remove(this);
		}
	}

	if (PreviewActor.IsValid())
	{
		PreviewActor->Destroy();
	}
}

void USpawnerComponent::InitPreview()
{
	// 기존 미리보기 엑터가 있다면 제거
	if (PreviewActor.IsValid())
	{
		PreviewActor->Destroy();
		PreviewActor.Reset();
	}

	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();

	if (!Owner || !World)
	{
		return;
	}
	UE_LOG(FCSpawner, Display, TEXT("%s InitPreview PASS 1"), *GetName());

	// 무한 루프 / 중복 생성 방지
	if (!ActorClass || ActorClass == Owner->GetClass())
	{
		return;
	}
	UE_LOG(FCSpawner, Display, TEXT("%s InitPreview PASS 2"), *GetName());

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	Params.Name = FName(*FString::Printf(TEXT("%s_%s"), *GetName(), *ActorClass->GetName()));
	Params.ObjectFlags |= RF_Transient | RF_TextExportTransient;
	Params.bHideFromSceneOutliner = true;

	if (ASpawnerPreviewActor* NewActor = World->SpawnActor<ASpawnerPreviewActor>())
	{
		UE_LOG(FCSpawner, Display, TEXT("%s InitPreview PASS 3"), *GetName());

		USceneComponent* PreviewRoot = NewActor->GetRootComponent();
		// 루트 컴포넌트 등록. 에디터 시간에 SpawnActor로 생성된 엑터의 컴포넌트는 아직 등록되지 않은 상태다.
		PreviewRoot->RegisterComponent();

		NewActor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		NewActor->OwnerSpawner = this;

		// ActorClass CDO 엑터의 컴포넌트 복제용 임시 엑터 생성
		// 에디터 시점에서 CDO 엑터의 Component는 등록되지 않은 상태기 때문에, 컴포넌트를 가져오려면 임시 엑터 생성 후  RegisterAllComponents()를 호출해줘야 한다.
		AActor* TempActor = World->SpawnActor<AActor>(ActorClass);
		TempActor->RegisterAllComponents();

		// PrimitiveComponent 기반 컴포넌트들의 참조 저장
		TArray<UPrimitiveComponent*> PrimitiveComps;
		TempActor->GetComponents(UPrimitiveComponent::StaticClass(), PrimitiveComps);

		// PrimitiveComps를 복제하여 미리보기 엑터에 추가
		for (UPrimitiveComponent* const& PrimitiveComp : PrimitiveComps)
		{
			UPrimitiveComponent* NewComp = DuplicateObject<UPrimitiveComponent>(PrimitiveComp, NewActor);
			NewComp->Mobility = EComponentMobility::Movable;
			NewActor->AddInstanceComponent(NewComp);
			NewActor->AddOwnedComponent(NewComp);
			NewComp->RegisterComponent();

			NewComp->AttachToComponent(PreviewRoot, FAttachmentTransformRules::KeepRelativeTransform);
		}

		// 임시 엑터 제거
		TempActor->Destroy();

		PreviewActor = NewActor;
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
	if (ActorClass == nullptr)
	{
		return;
	}
	
	if (CurrentTime > SpawnTime)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = bAttached ? GetOwner() : nullptr;
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
