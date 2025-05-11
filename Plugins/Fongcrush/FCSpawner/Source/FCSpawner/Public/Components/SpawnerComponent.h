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
	meta = ( BlueprintSpawnableComponent)
)
class FCSPAWNER_API USpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

	friend class FFCSpawnerComponentVisualizer;
	friend class USpawnerSubSystem;
	friend class FFCSpawnerEdMode;
	
public:
	USpawnerComponent();
#if WITH_EDITOR
	virtual void Reset();
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	/** 생성 클래스 변경시 기존 미리보기 제거 후 새 것 생성 */	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostLoad() override;
	/** 생성 클래스가 있다면 미리보기 생성. OnComponentCreated 에서는 SpawnActor 가 실패함. */
	virtual void OnRegister() override;
	/** 정리 */
	virtual void OnUnregister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	virtual void BeginDestroy() override;
	void CreateTemplate();
	void DestroyTemplate();

	/** Template을 갱신 */
	void UpdateTemplate();

	virtual void UpdatePreview();
	virtual void DestroyPreview();
	
	FORCEINLINE void MarkSelectInEdMode(const bool& bNew) { bIsSelectedInEdMode = bNew; }
	FORCEINLINE bool IsSelectedInEdMode() const { return bIsSelectedInEdMode; }

#endif

	/** 수동으로 Actor를 생성 */
	UFUNCTION(BlueprintCallable, Category = Spawner)
	AActor* Spawn();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FString GetWorldTypeString(UObject* const& Object);

	FORCEINLINE UClass* GetSpawnActorClass() const { return ActorClass; }
	FORCEINLINE AActor* GetActorTemplate() const { return ActorTemplate; }
	FORCEINLINE float GetSpawnTime() const { return SpawnTime; }
	
protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> ActorClass;
	
	/** 템플릿 액터 */
	UPROPERTY(VisibleAnywhere, Category = Spawner, meta = (ShowInnerProperties))
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
	UClass* PreviousActorClass;
	
	/** 생성된 액터 인스턴스 */
	UPROPERTY(meta = (HideInComponentTree), Transient)
	TSet<TObjectPtr<UPrimitiveComponent>> PreviewComponents;

	UPROPERTY()
	bool bIsSelectedInEdMode;
#endif
};
