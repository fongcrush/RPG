// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpawnerComponent.h"
#include "Components/BillboardComponent.h"

#include "FCSpawner.h"

#if WITH_EDITOR
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
	if (World && (World->WorldType == EWorldType::Editor || World->WorldType == EWorldType::EditorPreview))
	{
		if (PropertyName == GET_MEMBER_NAME_CHECKED(USpawnerComponent, ActorClass))
		{
			NewPreview();
		}
	}
}

void USpawnerComponent::OnRegister()
{
	Super::OnRegister();

	UWorld* World = GetWorld();
	if (World && (World->WorldType == EWorldType::Editor || World->WorldType == EWorldType::EditorPreview))
	{
		if (UEditorWorldSpawnerSubSystem* SpawnerSubSystem = World->GetSubsystem<UEditorWorldSpawnerSubSystem>())
		{
			// SpawnerSubSystem 에 참조 등록
			SpawnerSubSystem->Spawners.Emplace(this);
		}

		NewPreview();
	}
}

void USpawnerComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	UWorld* World = GetWorld();
	if (World && (World->WorldType == EWorldType::Editor || World->WorldType == EWorldType::EditorPreview))
	{
		World->GetSubsystem<UEditorWorldSpawnerSubSystem>()->Spawners.Remove(this);
	}
	DestroyPreview();
}

void USpawnerComponent::NewPreview()
{
	DestroyPreview();
	
	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();

	if (!ActorClass || !Owner || !World || !(World->WorldType == EWorldType::Editor || World->WorldType == EWorldType::EditorPreview))
	{
		return;
	}
	
	USceneComponent* OwnerRoot = Owner->GetRootComponent();

	// 엑터 편집기가 열릴 때, "SpawnActor failed because we are running a ConstructionScript" 가 발생하여 TempActor 생성 불가
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bAllowDuringConstructionScript = true;

	// ActorClass CDO엑터의 컴포넌트 복제용 임시 엑터 생성
	// 에디터 시점에서 CDO엑터의 컴포넌트는 아직 등록되지 않아서 가져올 수 없으므로 임시 엑터를 생성하고 RegisterAllComponent를 호출해 가져온다.
	AActor* TempActor = World->SpawnActor<AActor>(ActorClass, SpawnParameters);
	if (TempActor == nullptr) return;
	TempActor->RegisterAllComponents();

	// PrimitiveComponent 기반 컴포넌트 목록 가져오기
	TArray<UPrimitiveComponent*> PrimitiveComps;
	TempActor->GetComponents(PrimitiveComps, true);

	// PrimitiveComps를 복제하여 미리보기 생성
	for (UPrimitiveComponent* const& PrimitiveComp : PrimitiveComps)
	{
		if (PrimitiveComp->IsA<UBillboardComponent>()) continue;
		
		UPrimitiveComponent* NewComp = DuplicateObject<UPrimitiveComponent>(PrimitiveComp, Owner, NAME_None);
		NewComp->SetExternalPackage(GetTransientPackage());
		NewComp->SetFlags(RF_Transient | RF_TextExportTransient);
		NewComp->SetIsVisualizationComponent(true); // 시각화 전용 컴포넌트. 월드 아웃아리너와 디테일 패널 목록에 표시되지 않음. 편집기 전용으로 설정 됨.
		NewComp->CreationMethod = EComponentCreationMethod::Instance;
		NewComp->Mobility = EComponentMobility::Movable;
		// NewComp->bSelectable = false;

		// EditorPreview 에서 오작동이 있으므로 Editor 분기로 제한
		if (World->WorldType == EWorldType::Editor)
		{
			// Owner->AddInstanceComponent(NewComp);
			Owner->AddOwnedComponent(NewComp);
		}

		NewComp->RegisterComponent();
		NewComp->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

		PreviewComponents.Emplace(NewComp);

		if (UEditorWorldSpawnerSubSystem* SpawnerSubSystem = World->GetSubsystem<UEditorWorldSpawnerSubSystem>())
		{
			// 클릭 판별용 맵에 미리보기 컴포넌트 등록
			SpawnerSubSystem->PreviewMap.Emplace(NewComp, this);
		}
	}
	
	// 임시 엑터 제거
	TempActor->Destroy();
}

void USpawnerComponent::Reset()
{
	DestroyPreview();
	SpawnTime = 0.0f;
	CurrentTime = 0.0f;
	CountPolicy = ESpawnCountPolicy::Once;
	SpawnCount = 0;
	bAttached = false;
	ActorClass = nullptr;
}

void USpawnerComponent::DestroyPreview()
{
	UEditorWorldSpawnerSubSystem* SpawnerSubSystem = nullptr;
	if (UWorld* World = GetWorld())
	{
		SpawnerSubSystem = World->GetSubsystem<UEditorWorldSpawnerSubSystem>();
	}
	
	for (auto PreviewComp : PreviewComponents)
	{
		if (SpawnerSubSystem)
		{
			SpawnerSubSystem->PreviewMap.Remove(PreviewComp);
		}
		if (IsValid(PreviewComp))
		{
			PreviewComp->DestroyComponent();
			if (AActor* OwnerActor = GetOwner())
			{
				OwnerActor->RemoveInstanceComponent(PreviewComp);
			}
		}
	}
	PreviewComponents.Empty();
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
