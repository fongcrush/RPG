// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStackBase.h"
#include "Inventory.generated.h"

class UInventorySlotWidget;
class UInventoryWidget;
class ARPGCharacter;

/**
 * 
 */
UCLASS()
class RPG_API UInventory : public UItemStackBase
{
	GENERATED_BODY()
	friend UInventoryComponent;
	
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(VisibleInstanceOnly, Category = "Inventory")
	float Weight;
	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UInventory();
	virtual void Initialize() override;
	virtual void Use(ARPGCharacter* Character) override;
	virtual void LoadSlots();

	UFUNCTION(Category = "Inventory")
	bool RemoveExisting(UItemStackBase* ItemStack);
	
	UFUNCTION(Category = "Inventory")
	int32 RemoveItemQuantity(UItemStackBase* ItemStack, const int32 RequestedQuantity);
	
	UFUNCTION(Category = "Inventory")
	void SplitStack(UItemStackBase* Item, const int32 InQuantity);
	
	UFUNCTION(Category = "Inventory")
	void DropItemQuantity(AActor* Owner, UItemStackBase* DroppingStack, int32 DroppingQuantity) const;

	// Getters
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemStackBase*> GetContents() const { return Contents; }

	UFUNCTION(Category = "Inventory")
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<int32> GetFilledSlots() const { return FilledSlots; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<int32> GetEmptySlots() const { return EmptySlots; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE bool Contains(const UItemStackBase* ItemStack) const { return Contents.Contains(ItemStack); };

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY()
	TArray<TObjectPtr<UItemStackBase>> Contents;

	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY()
	TArray<int32> FilledSlots;
	
	UPROPERTY()
	TArray<int32> EmptySlots;

	TMap<TObjectPtr<UItemStackBase>, TObjectPtr<UInventorySlotWidget>> SlotMap;

	FInventoryStaticData* BagStaticData;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	FORCEINLINE FInventoryStaticData* GetBagStaticData() const { return BagStaticData; }
	FORCEINLINE TArray<TObjectPtr<UInventorySlotWidget>> GetSlots() const;
	FORCEINLINE TObjectPtr<UInventorySlotWidget> FindSlot(const TObjectPtr<UItemStackBase>& ItemStack) const { return *SlotMap.Find(ItemStack); }
	FORCEINLINE TObjectPtr<UInventorySlotWidget> FindNoneFullSlot(const TObjectPtr<UItemStackBase>& ItemStack) const;

	void AddStack(const TObjectPtr<UItemStackBase>& ItemStack, const int32& InQuantity);
};
using InventoryPtr = TObjectPtr<UInventory>;
