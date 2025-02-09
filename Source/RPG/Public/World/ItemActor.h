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

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditAnywhere, Category = "Pickup | Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;
	
	UPROPERTY(EditAnywhere, Category = "Pickup | Item")
	int32 Quantity;
	
	UPROPERTY(EditAnywhere, Category = "Pickup | Item")
	FDataTableRowHandle ItemDataHandle;
	
	UPROPERTY()
	TObjectPtr<UItemBase> ItemReference;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void BeginPlay() override;
	virtual void Interact(TObjectPtr<ARPGCharacter> InteractionCharacter) override;
	void TakePickup(const TObjectPtr<ARPGCharacter>& Taker);
	void UpdateInteractableData();
	
public:
	AItemActor();

	void InitializePickup();
	void InitializeDrop(const TObjectPtr<UItemBase>& DropItem, int32 InQuantity);
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};