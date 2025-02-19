// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Inventory.generated.h"

class UMainInventoryMenu;
class UInventorySlotWidget;
class UInventoryWidget;
class ARPGCharacter;

/**
 * 
 */
UCLASS()
class RPG_API UInventory : public UItemBase
{
	GENERATED_BODY()
	
public:	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UInventory();
	virtual void Initialize() override;
	virtual void Use(ARPGCharacter* Character) override;
	virtual void InitializeWidget();
	virtual void ReloadItems();
	virtual void Drop(AActor* Owner, const int32 QuantityToDrop) override;
	
	bool AddStack(UItemBase* const& Item, const int32& Quantity);
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
	UMainInventoryMenu* MainInventoryMenu;

	FInventoryStaticData* InventoryStaticData;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	void SwapItems(UItemBase* ItemA, UItemBase* ItemB) const;
	
	FORCEINLINE FInventoryStaticData* GetBagStaticData() const { return InventoryStaticData; }
	FORCEINLINE TArray<TObjectPtr<UInventorySlotWidget>> GetSlots() const;
};

using InventoryPtr = TObjectPtr<UInventory>;
