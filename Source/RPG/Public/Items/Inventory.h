// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/ScrollBox.h"
#include "UIs/InventoryMenu.h"
#include "Inventory.generated.h"

class UInventoryMenu;
class UItemSlotWidget;
class UInventoryWidget;
class ARPGCharacter;
class UPanelWidget;

/**
 * 
 */
UCLASS()
class RPG_API UInventory : public UItemBase, public IDynamicItem
{
	GENERATED_BODY()
	
public:	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	// Native
	//
	UInventory();
	
	virtual void Use(AActor* Owner) override;
	virtual bool Posses();
	virtual bool InitializeWidget();
	virtual void Drop(AActor* const& Owner, const int32& QuantityToDrop) override;

	// Getters
	virtual float GetWeight() const override;
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	
protected:
	FORCEINLINE UPanelWidget* GetInventoryMenu() const { return InventoryMenu ? InventoryMenu->InventoryPanel : nullptr; }
	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY()
	TObjectPtr<UInventoryMenu> InventoryMenu;

	UPROPERTY()
	bool bIsOwned;
	
	FInventoryStaticData* InventoryStaticData;
};
