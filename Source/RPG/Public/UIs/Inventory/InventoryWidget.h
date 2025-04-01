// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// UE
#include "CoreMinimal.h"

// User Defined
#include "UIs/Others/MovingUserWidget.h"
#include "InventoryWidget.generated.h"

#define ADD_FAIL -1

class UItemBase;
class UItemSlotWidget;
class UUniformGridPanel;

/**
 * 
 */
UCLASS()
class RPG_API UInventoryWidget : public UMovingUserWidget
{
	GENERATED_BODY()

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓

	virtual void SynchronizeProperties() override;
	void MakeSlots();

public:	
	int32 AddStack(UItemBase* const& Item, const int32& Quantity);
	int32 AddExisting(UItemBase* const& Item, const int32& Quantity);
	bool RemoveExisting(UItemBase* const& Item);
	int32 RemoveQuantity(UItemBase* const& Item, const int32 Quantity);
	void SwapItems(UItemSlotWidget* const& SlotA, UItemSlotWidget* const& SlotB) const;
	
	// Getters
	FORCEINLINE UItemSlotWidget* FindSlot(const UItemBase* const& SearchingItem) const;
	FORCEINLINE UItemSlotWidget* FindNoneFullSlot(const UItemBase* const& SearchingItem) const;
	FORCEINLINE UItemSlotWidget* FindEmptySlot();
	FORCEINLINE bool Contains(const UItemBase* const& SearchingItem) const;
	float GetWeight() const;
	
protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UItemSlotWidget> SlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 SlotColumnSize;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 SlotRowSize;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryPanel;
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemSlotWidget>> ItemSlots;
};

using InventoryWidgetPtr = TObjectPtr<UInventoryWidget>;
