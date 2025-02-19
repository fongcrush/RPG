// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_Interaction.h"
#include "ItemActor.generated.h"

class UItemBase;

UCLASS()
class RPG_API AItemActor : public AActor, public IInterface_Interaction
{
	GENERATED_BODY()
	
public:
	AItemActor();
	void Initialize();
	void InitializeDrop(const TObjectPtr<UItemBase>& DropItem, int32 InQuantity);

	virtual void BeginFocus() override { if (PickupMesh) PickupMesh->SetRenderCustomDepth(true); }
	virtual void EndFocus() override { if (PickupMesh) PickupMesh->SetRenderCustomDepth(false); }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> PickupMesh;
	
	UPROPERTY(EditAnywhere, meta=(BlueprintBaseOnly))
	TObjectPtr<UItemBase> Item;
	
	UPROPERTY(EditAnywhere, meta=(ClampMin = 1))
	int32 Quantity;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void BeginPlay() override;
	virtual void Interact(ARPGCharacter* const& InteractionCharacter) override { if (InteractionCharacter) TakePickup(InteractionCharacter); }
	void TakePickup(const TObjectPtr<ARPGCharacter>& Taker);
	void UpdateInteractableData();
};