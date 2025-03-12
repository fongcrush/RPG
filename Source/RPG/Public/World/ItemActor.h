// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interaction.h"
#include "ItemActor.generated.h"

class UInventoryComponent;
class ARPGHUD;
class UItemBase;

UCLASS()
class RPG_API AItemActor : public AActor, public IInteraction
{
	GENERATED_BODY()
	
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	AItemActor();
	
	void InitializeDrop(const TObjectPtr<UItemBase>& ItemDropped, int32 InQuantity);

	virtual void BeginFocus() override; 
	virtual void EndFocus() override;
	virtual void Interact(APawn* const& Interactor) override;
	virtual void EndInteract() override;
	
	virtual FInteractableData GetInteractableData() const override { return InteractableData; }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	virtual void BeginPlay() override;
	void LoadInteractableData();
	void TakeItem(APlayerCharacter* const& Taker);

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> PickupMesh;
	
	UPROPERTY(EditAnywhere, meta=(BlueprintBaseOnly))
	TSubclassOf<UItemBase> ItemClass;
	
	UPROPERTY()
	TObjectPtr<UItemBase> Item;
	
	UPROPERTY(EditAnywhere, meta=(ClampMin = 1))
	int32 Quantity;

	TObjectPtr<ARPGHUD> HUD;

	FInteractableData InteractableData;
};