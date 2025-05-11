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
	if (ActorTemplate)
	{
		USpawnerSubSystem* SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>();
		SpawnerSubSystem->Spawners.Remove(this);
		SpawnerSubSystem->TemplateToSpawnerMap.Remove(ActorTemplate);
		DestroyTemplate();
	}
	ActorClass = nullptr;
	PreviousActorClass = nullptr;
	SpawnTime = 0.0f;
	CurrentTime = 0.0f;
	CountPolicy = ESpawnCountPolicy::Once;
	SpawnCount = 0;
	bAttached = false;
}

void USpawnerComponent::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);

	if (PropertyAboutToChange &&
		PropertyAboutToChange->GetFName() == GET_MEMBER_NAME_CHECKED(USpawnerComponent, ActorClass))
	{
		PreviousActorClass = ActorClass;
	}
}

void USpawnerComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(USpawnerComponent, ActorClass) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(USpawnerComponent, ActorTemplate))
	{
		UpdateTemplate();
	}
}

void USpawnerComponent::PostLoad()
{
	Super::PostLoad();

	if (USpawnerSubSystem* SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>())
	{
		SpawnerSubSystem->Spawners.Add(this);
		UpdateTemplate();
	}
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
		SpawnerSubSystem->TemplateToSpawnerMap.Remove(ActorTemplate);
	}
	
	Super::OnUnregister();
}

void USpawnerComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	DestroyPreview();
	if (USpawnerSubSystem* SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>())
	{
		SpawnerSubSystem->Spawners.Remove(this);
		SpawnerSubSystem->TemplateToSpawnerMap.Remove(ActorTemplate);
	}
	DestroyTemplate();

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void USpawnerComponent::BeginDestroy()
{
	DestroyPreview();
	if (USpawnerSubSystem* SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>())
	{
		SpawnerSubSystem->Spawners.Remove(this);
		SpawnerSubSystem->TemplateToSpawnerMap.Remove(ActorTemplate);
	}
	DestroyTemplate();

	Super::BeginDestroy();
}

void USpawnerComponent::UpdateTemplate()
{
	USpawnerSubSystem* SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>();
	if (PreviousActorClass && PreviousActorClass != ActorClass)
	{
		SpawnerSubSystem->TemplateToSpawnerMap.Remove(PreviousActorClass);
		PreviousActorClass = nullptr;
	}

	if (ActorClass)
	{
		// 새 템플릿 생성
		if (ActorTemplate == nullptr)
		{
			CreateTemplate();
			SpawnerSubSystem->TemplateToSpawnerMap.Add(ActorTemplate, this);
		}
		else if (ActorTemplate->GetClass() != ActorClass)
		{
			SpawnerSubSystem->TemplateToSpawnerMap.Remove(ActorTemplate);
			DestroyTemplate();

			CreateTemplate();
			SpawnerSubSystem->TemplateToSpawnerMap.Add(ActorTemplate, this);
		}
	}
	else if (ActorTemplate)
	{
		SpawnerSubSystem->TemplateToSpawnerMap.Remove(ActorTemplate);
		DestroyTemplate();
	}
}

void USpawnerComponent::CreateTemplate()
{
	UWorld* World = GetWorld();
	AActor* Owner = GetOwner();

	ULevel* LevelToSpawnIn = World->PersistentLevel;
	UPackage* LevelPackage = nullptr;
	UPackage* TransientPackage = GetTransientPackage();

	const FName BaseName = *FString::Printf(TEXT("%s_%s_%s_Template"), Owner ? *Owner->GetName() : *FString("Null"), *GetName(), *ActorClass->GetName());
	const FName TemplateName = MakeUniqueObjectName(LevelToSpawnIn, ActorClass, BaseName);

	constexpr EObjectFlags TemplateFlags = RF_ArchetypeObject | RF_Transactional | RF_Public;

	ActorTemplate = NewObject<AActor>(TransientPackage,
	                                  ActorClass,
	                                  TemplateName,
	                                  TemplateFlags,
	                                  nullptr, false, nullptr,
	                                  Owner->GetPackage());
	//ActorTemplate->SetExternalPackage(GetPackage());
}

void USpawnerComponent::DestroyTemplate()
{
	// ActorTemplate->Rename(nullptr, GetTransientPackage(), REN_DontCreateRedirectors);
	ActorTemplate->SetExternalPackage(nullptr);
	ActorTemplate->MarkAsGarbage();
	ActorTemplate = nullptr;
}

void USpawnerComponent::UpdatePreview()
{
	DestroyPreview();

	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();

	if (IsTemplate() || !ActorClass || !Owner || !World || !(World->WorldType == EWorldType::Editor || World->WorldType == EWorldType::EditorPreview))
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

		// EditorPreview 에서 오작동이 있으므로 Editor 분기로 제한. 필수.
		if (World->WorldType == EWorldType::Editor)
		{
			FName("", 60);
		}
		else if (World->WorldType == EWorldType::EditorPreview)
		{
		}

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


FString USpawnerComponent::GetWorldTypeString(UObject* const& Object)
{
	if (IsValid(Object))
	{
		if (UWorld* World = Object->GetWorld())
		{
			FString WorldName = World->GetFullName();
			switch (World->WorldType)
			{
			case EWorldType::None: return "None(" + WorldName + ")";
			case EWorldType::Game: return "Game(" + WorldName + ")";
			case EWorldType::Editor: return "Editor(" + WorldName + ")";
			case EWorldType::PIE: return "PIE(" + WorldName + ")";
			case EWorldType::EditorPreview: return "EditorPreview(" + WorldName + ")";
			case EWorldType::GamePreview: return "GamePreview(" + WorldName + ")";
			case EWorldType::GameRPC: return "GameRPC(" + WorldName + ")";
			case EWorldType::Inactive: return "Inactive(" + WorldName + ")";
			default: break;
			}
		}
	}
	return "InValid";
}