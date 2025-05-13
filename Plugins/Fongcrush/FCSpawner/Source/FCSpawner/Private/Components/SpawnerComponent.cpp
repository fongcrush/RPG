// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpawnerComponent.h"

#include "FCSpawner.h"

#if WITH_EDITOR
#include "SubSystems/SpawnerSubSystem.h"
#include "Components/BillboardComponent.h"
#endif


USpawnerComponent::USpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;

	SpawnTime = 0;
	CurrentTime = 0;
	CountPolicy = ESpawnCountPolicy::Once;
	SpawnCount = 0;
	bAttached = false;
	bIsSelectedInEdMode = false;
}

#if WITH_EDITOR

void USpawnerComponent::Reset()
{
	DestroyPreview();
	DestroyTemplate();
	ActorClass = nullptr;
	SpawnTime = 0.0f;
	CurrentTime = 0.0f;
	CountPolicy = ESpawnCountPolicy::Once;
	SpawnCount = 0;
	bAttached = false;
}

void USpawnerComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(USpawnerComponent, ActorClass))
	{
		DestroyTemplate();
		if (ActorClass)
		{
			CreateTemplate();
			UpdatePreview();
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(USpawnerComponent, ActorTemplate) && IsRegistered())
	{
		UpdatePreview();
	}
}

void USpawnerComponent::CreateTemplate()
{
	AActor* Owner = GetOwner();

	const FName BaseName = *FString::Printf(TEXT("%s_%s_%s_Template"), Owner ? *Owner->GetName() : *FString("Null"), *GetName(), *ActorClass->GetName());
	const FName TemplateName = MakeUniqueObjectName(this, ActorClass, BaseName);

	constexpr EObjectFlags TemplateFlags = RF_ArchetypeObject | RF_Transactional | RF_Public;
	ActorTemplate = NewObject<AActor>(this, ActorClass, TemplateName, TemplateFlags);
}

void USpawnerComponent::OnRegister()
{
	Super::OnRegister();

	if (USpawnerSubSystem* SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>())
	{
		SpawnerSubSystem->Spawners.Add(this);
		UpdatePreview();
	}
}

void USpawnerComponent::OnUnregister()
{
	DestroyPreview();
	if (USpawnerSubSystem* SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>())
	{
		SpawnerSubSystem->Spawners.Remove(this);
	}

	Super::OnUnregister();
}

void USpawnerComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	DestroyPreview();
	if (USpawnerSubSystem* SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>())
	{
		SpawnerSubSystem->Spawners.Remove(this);
	}
	DestroyTemplate();

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void USpawnerComponent::DestroyTemplate()
{
	if (IsValid(ActorTemplate))
	{
		ActorTemplate->Rename(nullptr, GetTransientPackage(), REN_DontCreateRedirectors);
		ActorTemplate = nullptr;
	}
	DestroyPreview();
}

void USpawnerComponent::UpdatePreview()
{
	DestroyPreview();

	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();
	
	if (!ActorClass || !Owner || !World || !(World->WorldType == EWorldType::Editor || World->WorldType == EWorldType::EditorPreview))
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	// 엑터 편집기가 열릴 때 발생하는 "SpawnActor failed because we are running a ConstructionScript" 에러 방지
	SpawnParameters.bAllowDuringConstructionScript = true;
	SpawnParameters.Template = ActorTemplate;

	// ActorClass CDO엑터의 컴포넌트 복제용 임시 엑터 생성
	// 에디터 시점에서 CDO엑터의 컴포넌트는 아직 등록되지 않아서 가져올 수 없으므로 임시 엑터를 생성하고 RegisterAllComponent를 호출해 가져온다.
	AActor* TempActor = World->SpawnActor<AActor>(ActorClass, SpawnParameters);
	if (TempActor == nullptr) return;
	TempActor->Rename(nullptr, nullptr, REN_DoNotDirty | REN_DontCreateRedirectors);
	TempActor->ClearActorLabel();

	// PrimitiveComponent 기반 컴포넌트 목록 가져오기
	TArray<UPrimitiveComponent*> PrimitiveComps;
	TempActor->GetComponents(PrimitiveComps, true);

	// PrimitiveComps를 복제하여 미리보기 생성
	for (UPrimitiveComponent* const& PrimitiveComp : PrimitiveComps)
	{
		if (PrimitiveComp->IsA<UBillboardComponent>()) continue;

		UPrimitiveComponent* NewComp = DuplicateObject<UPrimitiveComponent>(PrimitiveComp, this);
		NewComp->SetExternalPackage(GetTransientPackage());
		NewComp->SetFlags(RF_Transient | RF_TextExportTransient | RF_DuplicateTransient);
		NewComp->SetIsVisualizationComponent(true); // 월드 아웃아리너와 디테일 패널 목록에 표시x, 편집기 전용으로 설정
		NewComp->Mobility = EComponentMobility::Movable;

		Owner->AddOwnedComponent(NewComp);

		NewComp->RegisterComponent();
		NewComp->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

		PreviewComponents.Add(NewComp);

		if (USpawnerSubSystem* SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>())
		{
			// 클릭 판별용 맵에 미리보기 컴포넌트 등록
			SpawnerSubSystem->PreviewMap.Add(NewComp, this);
		}
	}

	// 임시 엑터 제거
	if (!World->EditorDestroyActor(TempActor, false))
	{
		UE_LOG(FCSpawner, Display, TEXT("%s 파괴 실패"), *TempActor->GetFullName())
	}
}

void USpawnerComponent::DestroyPreview()
{
	USpawnerSubSystem* SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>();

	for (auto PreviewComp : PreviewComponents)
	{
		if (SpawnerSubSystem) SpawnerSubSystem->PreviewMap.Remove(PreviewComp);
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

AActor* USpawnerComponent::Spawn()
{
	UWorld* World = GetWorld();
	if (!World || !ActorClass)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bNoFail = true;

	if (ActorTemplate && ActorTemplate->GetClass() == ActorClass)
	{
		SpawnParams.Template = ActorTemplate;
	}

	if (AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClass, GetComponentTransform(), SpawnParams))
	{
		if (bAttached)
		{
			SpawnedActor->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
		return SpawnedActor;
	}
	return nullptr;
}

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
		Spawn();

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
