// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStackBase.h"
#include "Inventory.generated.h"

class UItemSlotWidget;
class UInventoryWidget;
class ARPGCharacter;

/**
 * 
 */
UCLASS()
class RPG_API UInventory : public UItemStackBase
{
	GENERATED_BODY()
	using TItemStackPtr = TObjectPtr<UItemStackBase>;
	using TItemSlotPtr = TObjectPtr<UItemSlotWidget>;
	
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(VisibleInstanceOnly, Category = "Inventory")
	float TotalWeight;
	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UInventory();
	virtual void Initialize() override;
	virtual void Use(ARPGCharacter* Character) override;
	virtual void RegisterInventory(UInventoryComponent* Inventory);
	virtual void LoadSlots();

	UFUNCTION(Category = "Inventory")
	void RemoveStack(UItemStackBase* ItemStack);
	
	UFUNCTION(Category = "Inventory")
	int32 RemoveStackQuantity(UItemStackBase* Item, const int32 RequestedQuantity);
	
	UFUNCTION(Category = "Inventory")
	void SplitStack(UItemStackBase* Item, const int32 Quantity);

	// Getters
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<TItemStackPtr> GetContents() const { return Contents; }

	UFUNCTION(Category = "Inventory")
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return BagStaticData->WeightCapacity; };
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TMap<TItemStackPtr,TItemSlotPtr> GetInventoryMap() const { return InventoryMap; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<int32> GetFilledSlots() const { return FilledSlots; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<int32> GetEmptySlots() const { return EmptySlots; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE bool Contains(const TObjectPtr<UItemStackBase>& ItemStack) const { return InventoryMap.Contains(ItemStack); };

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY()
	TArray<TItemStackPtr> Contents;

	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY()
	TMap<TItemStackPtr,TItemSlotPtr> InventoryMap;

	UPROPERTY()
	TArray<int32> FilledSlots;
	
	UPROPERTY()
	TArray<int32> EmptySlots;

	FBagStaticData* BagStaticData;


	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemStackBase* Item);
	FItemAddResult HandleAddNoneStackable(const TObjectPtr<UItemStackBase>& Item);
	FItemAddResult HandleAddStackable(const TObjectPtr<UItemStackBase>& ItemStack, const int32 RequestedQuantity);
	void AddNewStack(const TObjectPtr<UItemStackBase>& ItemStack, int32 Quantity);
	void SetWeight(const float& InWeight);
	
	FORCEINLINE UItemStackBase* FindNoneFullStack(const UItemStackBase* ItemStack) const;
	FORCEINLINE UItemSlotWidget* FindSlot(const TObjectPtr<UItemStackBase>& ItemStack) const { return *InventoryMap.Find(ItemStack); }
	FORCEINLINE int32 GetItemQuantityCanAdd(const TObjectPtr<UItemStackBase>& Item, int32 Quantity) const;	
};
