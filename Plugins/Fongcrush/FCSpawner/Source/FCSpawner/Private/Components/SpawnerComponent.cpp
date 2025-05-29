// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpawnerComponent.h"

#include "FCSpawner.h"

#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#endif

TSet<TWeakObjectPtr<USpawnerComponent>> USpawnerComponent::AllRegistered;
TMap<UPrimitiveComponent*, TWeakObjectPtr<USpawnerComponent>> USpawnerComponent::PreviewToSpawnerMap;
TMultiMap<UObject*, TWeakObjectPtr<USpawnerComponent>> USpawnerComponent::ClassToSpawnersMap;


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

void USpawnerComponent::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);
	if (PropertyAboutToChange->GetFName() == GET_MEMBER_NAME_CHECKED(USpawnerComponent, ActorClass))
	{
		DestroyTemplate();
		ClassToSpawnersMap.Remove(ActorClass, this);
	}
}

void USpawnerComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(USpawnerComponent, ActorClass))
	{
		if (ActorClass)
		{
			if (IsRegistered())
			{
				ClassToSpawnersMap.AddUnique(ActorClass, this);
			}
			CreateTemplate();
			UpdatePreview();
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(USpawnerComponent, ActorTemplate) && IsRegistered())
	{
		UpdatePreview();
	}
}

void USpawnerComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (ActorClass && ActorTemplate)
	{
		UObject* Owner = GetOwner();
		const FName BaseName = *FString::Printf(TEXT("%s_%s_%s_Template"), Owner ? *Owner->GetName() : *FString("None"), *GetName(), *ActorClass->GetName());
		const FName TemplateName = MakeUniqueObjectName(this, ActorClass, BaseName, EUniqueObjectNameOptions::GloballyUnique);
		ActorTemplate->Rename(*TemplateName.ToString(), this, REN_DontCreateRedirectors);
	}
}

void USpawnerComponent::CreateTemplate()
{
	UObject* Owner = GetOwner();
	const FName BaseName = *FString::Printf(TEXT("%s_%s_%s_Template"), Owner ? *Owner->GetName() : *FString("None"), *GetName(), *ActorClass->GetName());
	const FName TemplateName = MakeUniqueObjectName(this, ActorClass, BaseName, EUniqueObjectNameOptions::GloballyUnique);

	constexpr EObjectFlags TemplateFlags = RF_ArchetypeObject | RF_Transactional | RF_Public;
	ActorTemplate = NewObject<AActor>(this, ActorClass, TemplateName, TemplateFlags);
}

void USpawnerComponent::OnRegister()
{
	Super::OnRegister();

	UpdatePreview();
	AllRegistered.Add(this);
	if (ActorClass)
	{
		// UE_LOG(FCSpawner, Display, TEXT("ActorClass: %p"), ActorClass.Get())
		ClassToSpawnersMap.AddUnique(ActorClass, this);
	}
}

void USpawnerComponent::OnUnregister()
{
	// UE_LOG(FCSpawner, Display, TEXT("OnUnregister: %s: (%p) %s"), *GetWorldTypeString(this), this, *GetFullName());
	UWorld* World = GetWorld();
	if (World && World->WorldType == EWorldType::EditorPreview)
	{
		DestroyTemplate();
	}
	else
	{
		DestroyPreview();
	}
	AllRegistered.Remove(this);
	if (ActorClass)
	{
		ClassToSpawnersMap.Remove(ActorClass, this);
	}

	Super::OnUnregister();
}

void USpawnerComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	DestroyTemplate();
	AllRegistered.Remove(this);
	if (ActorClass)
	{
		ClassToSpawnersMap.Remove(ActorClass, this);
	}

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void USpawnerComponent::DestroyTemplate()
{
	DestroyPreview();

	if (IsValid(ActorTemplate))
	{
		if (IsRegistered())
		{
			ActorTemplate->ClearActorLabel();
			GetWorld()->EditorDestroyActor(ActorTemplate, false);
		}
		else
		{
			ActorTemplate->Rename(*FString(""), GetTransientPackage(), REN_DontCreateRedirectors);
		}
		ActorTemplate = nullptr;
	}
}

void USpawnerComponent::UpdatePreview()
{
	DestroyPreview();

	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();

	if (!IsRegistered() || !ActorClass || !Owner || !World || !(World->WorldType == EWorldType::Editor || World->WorldType == EWorldType::EditorPreview))
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	// 엑터 편집기가 열릴 때의 에러 방지
	SpawnParameters.bAllowDuringConstructionScript = true;
	SpawnParameters.Template = ActorTemplate;
	SpawnParameters.Name = *FString::Printf(TEXT("%s_%s_%s_TempActor"), Owner ? *Owner->GetName() : *FString("None"), *GetName(), *ActorClass->GetName());
	SpawnParameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParameters.bCreateActorPackage = false;
	SpawnParameters.ObjectFlags = RF_Transient | RF_TextExportTransient | RF_DuplicateTransient;

	AActor* TempActor = World->SpawnActor<AActor>(ActorClass, SpawnParameters);
	if (!TempActor)
	{
		return;
	}

	TArray<UPrimitiveComponent*> Primitives;
	TempActor->GetComponents(Primitives);

	// PrimitiveComps를 복제하여 미리보기 생성
	for (UPrimitiveComponent* const& Primitive : Primitives)
	{
		if (Primitive->IsA<UBillboardComponent>())
		{
			continue;
		}

		// 갱신이 적용되도록 ClassDefault를 복제
		UPrimitiveComponent* NewPrimitive = DuplicateObject<UPrimitiveComponent>(Primitive, this);
		NewPrimitive->SetExternalPackage(GetTransientPackage());
		NewPrimitive->SetFlags(RF_Transient | RF_TextExportTransient | RF_DuplicateTransient);
		NewPrimitive->SetIsVisualizationComponent(true); // 월드 아웃아리너와 디테일 패널 목록에 표시x, 편집기 전용으로 설정
		NewPrimitive->Mobility = EComponentMobility::Movable;

		Owner->AddOwnedComponent(NewPrimitive);
		Owner->AddInstanceComponent(NewPrimitive);
		NewPrimitive->OnComponentCreated();
		NewPrimitive->RegisterComponent();
		NewPrimitive->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

		PreviewComponents.Add(NewPrimitive);

		// 클릭 판별용 전역 맵에 미리보기 컴포넌트 등록
		PreviewToSpawnerMap.Add(NewPrimitive, this);
	}

	// 임시 엑터 제거
	if (!World->EditorDestroyActor(TempActor, false))
	{
		UE_LOG(FCSpawner, Display, TEXT("%s 파괴 실패"), *TempActor->GetFullName())
	}
}

void USpawnerComponent::DestroyPreview()
{
	for (auto PreviewComp : PreviewComponents)
	{
		PreviewToSpawnerMap.Remove(PreviewComp); // 전역 맵
		if (IsValid(PreviewComp))
		{
			PreviewComp->DestroyComponent();
		}
	}
	PreviewComponents.Empty();
}

#endif

AActor* USpawnerComponent::SpawnFromTemplate()
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
		SpawnFromTemplate();

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
