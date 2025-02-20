// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/ScrollBox.h"
#include "UIs/InventoryMenu.h"
#include "Inventory.generated.h"

class UInventoryMenu;
class UInventorySlotWidget;
class UInventoryWidget;
class ARPGCharacter;

#define ADD_FAIL -1

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
	UInventory();
	virtual void Use(AActor* Owner) override;
	virtual bool Posses();
	virtual bool InitializeWidget();
	virtual void ReloadItems();
	virtual void Drop(AActor* Owner, const int32 QuantityToDrop) override;
	
	int32 AddStack(UItemBase* const& Item, const int32& Quantity);
	int32 AddExisting(UItemBase* const& Item, const int32& Quantity);
	bool RemoveExisting(UItemBase* Item);
	int32 RemoveQuantity(UItemBase* Item, const int32 Quantity);

	// Getters
	FORCEINLINE UInventorySlotWidget* FindSlot(const UItemBase* const& SearchingItem) const;
	FORCEINLINE UInventorySlotWidget* FindNoneFullSlot(const UItemBase* const& SearchingItem) const;
	FORCEINLINE UInventorySlotWidget* FindEmptySlot() const;
	FORCEINLINE TArray<UItemBase*> GetItems() const { TArray<UItemBase*> Items; ItemSlotMap.GetKeys(Items); return Items; }
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	FORCEINLINE bool Contains(const UItemBase* Item) const { return ItemSlotMap.Contains(Item); }
	FORCEINLINE virtual float GetWeight() const override;

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY()
	TMap<UItemBase*, UInventorySlotWidget*> ItemSlotMap;

	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY()
	UInventoryMenu* InventoryMenu;
	
	FInventoryStaticData* InventoryStaticData;

	UPROPERTY()
	bool bIsOwned;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	void SwapItems(UItemBase* ItemA, UItemBase* ItemB) const;
	FORCEINLINE TArray<TObjectPtr<UInventorySlotWidget>> GetSlots() const;
	FORCEINLINE UPanelWidget* GetInventoryPanel() const { return InventoryMenu ? InventoryMenu->InventoryPanel : nullptr; }
};
using InventoryPtr = TObjectPtr<UInventory>;
