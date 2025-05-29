// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Components/SceneComponent.h"
#include "SpawnerComponent.generated.h"

UENUM()
enum class ESpawnCountPolicy : uint8
{
	Once,
	Constant,
	Infinite,
};

UCLASS(
	BlueprintType,
	PrioritizeCategories = ("Transform Spawner"),
	hideCategories = (
		Variable,
		Trigger,
		PhysicsVolume,
		ComponentTick,
		Tick,
		Tags,
		ComponentReplication,
		Activation,
		Physics,
		LOD,
		AssetUserData,
		Navigation,
		ArrowComponent,
		HLOD,
		Rendering,
		Mobile,
		RayTracing,
		Replication
	),
	meta = (BlueprintSpawnableComponent)
)
class FCSPAWNER_API USpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

	friend class FFCSpawnerModule;
	friend class FFCSpawnerComponentVisualizer;
	friend class USpawnerSubSystem;
	friend class FFCSpawnerEdMode;
	
public:
	USpawnerComponent();
#if WITH_EDITOR
	/** 컨텍스트 메뉴에서 사용 */
	virtual void Reset();
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	/** 생성 클래스 변경시 기존 미리보기 제거 후 새 것 생성 */	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnComponentCreated() override;
	/** 생성 클래스가 있다면 미리보기 생성. OnComponentCreated 에서는 SpawnActor 가 실패함. */
	virtual void OnRegister() override;
	/** 객체 정리 */
	virtual void OnUnregister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	void CreateTemplate();
	void DestroyTemplate();

	virtual void UpdatePreview();
	virtual void DestroyPreview();
	
	FORCEINLINE void MarkSelectInEdMode(const bool& bNew) { bIsSelectedInEdMode = bNew; }
	FORCEINLINE bool IsSelectedInEdMode() const { return bIsSelectedInEdMode; }

#endif

	/** 수동으로 Actor를 생성 */
	UFUNCTION(BlueprintCallable, Category = Spawner)
	AActor* SpawnFromTemplate();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE UClass* GetSpawnActorClass() const { return ActorClass; }
	FORCEINLINE AActor* GetActorTemplate() const { return ActorTemplate; }
	FORCEINLINE float GetSpawnTime() const { return SpawnTime; }

	FORCEINLINE static const TSet<TWeakObjectPtr<USpawnerComponent>>& GetAllSpawners() { return AllRegistered; }
	static USpawnerComponent* FindSpawnerFromPreview(const UPrimitiveComponent* const& Preview)
	{
		TWeakObjectPtr<USpawnerComponent>* SpawnerWeakPtr = PreviewToSpawnerMap.Find(Preview);
		if (SpawnerWeakPtr && SpawnerWeakPtr->IsValid())
		{
			return SpawnerWeakPtr->Get();
		}
		return nullptr;
	}

	static int32 GetSelectedSpawners(TArray<UObject*>& OutSelected)
	{
		OutSelected.Reserve(AllRegistered.Num());
		for (auto& Spawner : AllRegistered)
		{
			if (Spawner.IsValid() && Spawner->IsSelectedInEdMode())
			{
				OutSelected.Emplace(Spawner.Get());
			}
		}
		return OutSelected.Num();
	}
	
protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> ActorClass;
	
	/** 템플릿 액터 */
	UPROPERTY(EditAnywhere, Category = "Spawner", Instanced,  meta = (ShowInnerProperties))
	TObjectPtr<AActor> ActorTemplate;
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	float SpawnTime;

	UPROPERTY(VisibleInstanceOnly, Category = "Spawner")
	float CurrentTime;
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	ESpawnCountPolicy CountPolicy;
	
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (EditCondition = "CountPolicy == ESpawnCountPolicy::Constant", EditConditionHides))
	uint32 SpawnCount;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	bool bAttached;

#if WITH_EDITORONLY_DATA	
	/** 생성된 액터 인스턴스 */
	UPROPERTY(meta = (HideInComponentTree), Transient)
	TSet<TObjectPtr<UPrimitiveComponent>> PreviewComponents;

	UPROPERTY()
	bool bIsSelectedInEdMode;

	/** 모든 Spawner를 갖는 전역 Set */
	static TSet<TWeakObjectPtr<USpawnerComponent>> AllRegistered;
	/** 미리보기 클릭 판별 용도의 전역 맵 */
	static TMap<UPrimitiveComponent*, TWeakObjectPtr<USpawnerComponent>> PreviewToSpawnerMap;
	/** 템플릿의 디폴트 엑터 컴파일 시 프리뷰 갱신 용도의 전역 맵 */
	static TMultiMap<UObject*, TWeakObjectPtr<USpawnerComponent>> ClassToSpawnersMap;
#endif
};
