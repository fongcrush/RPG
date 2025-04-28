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
	friend class UEditorWorldSpawnerSubSystem;
	friend class FFCSpawnerEdMode;
	
public:
	USpawnerComponent();
#if WITH_EDITOR
	/** 생성 클래스 변경시 기존 미리보기 제거 후 새 것 생성 */	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	/** 생성 클래스가 바인딩 된 상태라면 미리보기 생성. OnComponentCreated 에서는 SpawnActor 가 실패함. */
	virtual void OnRegister() override;
	/** 미리보기 삭제 */
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	/** 미리보기 생성 */ 
	virtual void NewPreview();
	virtual void Reset();
	virtual void DestroyPreview();
	FORCEINLINE void MarkSelectInEdMode(const bool& bNew) { bIsSelectedInEdMode = bNew; }
	FORCEINLINE bool IsSelectedInEdMode() const { return bIsSelectedInEdMode; }
#endif
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE UClass* GetSpawnActorClass() const { return ActorClass; }
	FORCEINLINE float GetSpawnTime() const { return SpawnTime; }
	
protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> ActorClass;

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
	UPROPERTY(meta = (HideInComponentTree))
	TSet<TObjectPtr<UPrimitiveComponent>> PreviewComponents;

private:
	UPROPERTY()
	bool bIsSelectedInEdMode;
#endif
};
