// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "PickupActor.generated.h"

class UItemBase;

// 다중 아이템 인터랙션 구현 필요
USTRUCT()
struct FPickupInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle ItemDataHandle;

	UPROPERTY(EditAnywhere)
	int32 Quantity;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemBase> ItemReference;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
};

UCLASS()
class RPG_API APickupActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditAnywhere, Category = "Pickup | Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(EditAnywhere, Category = "Pickup | Item Reference")
	TArray<FPickupInfo> PickupInfos;

	FInteractableData DefaultInteractableData;


	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void BeginPlay() override;
	virtual void Interact(TObjectPtr<ARPGCharacter> InteractionCharacter) override;
	void TakePickup(const TObjectPtr<ARPGCharacter>& Taker);
	void UpdateInteractableData();
	
public:
	APickupActor();

	void InitializePickup();
	void InitializeDrop(const TObjectPtr<UItemBase>& DropItem, int32 InQuantity);
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	// virtual FInteractableData* GetInteractableData() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};