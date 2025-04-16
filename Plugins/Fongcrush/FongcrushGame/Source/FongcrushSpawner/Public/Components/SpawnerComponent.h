// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Components/ArrowComponent.h"
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
class FONGCRUSHSPAWNER_API USpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

	friend class FSpawnerComponentVisualizer;
	
public:
	USpawnerComponent();
#if WITH_EDITOR
	virtual void OnComponentCreated() override;
	
	virtual void SpawnPreview();
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

public:
#if WITH_EDITORONLY_DATA
	TWeakObjectPtr<AActor> PreviewActor;	
#endif
};
