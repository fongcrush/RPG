// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Components/ArrowComponent.h"
#include "SpawnComponent.generated.h"

UENUM()
enum class ESpawnCountPolicy : uint8
{
	Once,
	Constant,
	Infinite,
};

UCLASS(
	BlueprintType,
	PrioritizeCategories = ("Transform Spawn"),
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
class RPG_API USpawnComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	USpawnComponent();
#if WITH_EDITOR
	
#endif
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float SpawnTime;

	UPROPERTY(VisibleInstanceOnly, Category = "Spawn")
	float CurrentTime;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	ESpawnCountPolicy CountPolicy;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawn", meta = (EditCondition = "CountPolicy == ESpawnCountPolicy::Constant", EditConditionHides))
	uint32 SpawnCount;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	bool bAttached;
};
