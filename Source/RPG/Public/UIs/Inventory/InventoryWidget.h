// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// UE
#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

// User Defined
#include "InventorySlotWidget.h"
#include "Items/ItemBase.h"
#include "UIs/MovingUserWidget.h"

#include "InventoryWidget.generated.h"


class UInventory;
class UUniformGridPanel;
class UInventorySlotWidget;
class UInventoryComponent;
class ARPGCharacter;
class UTextBlock;
class UWrapBox;
/**
 * 
 */
UCLASS()
class RPG_API UInventoryWidget : public UMovingUserWidget
{
	GENERATED_BODY()

	friend UInventoryComponent;
	friend UInventory;

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInventorySlotWidget> SlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 SlotColumnSize;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 SlotRowSize;
	
	UPROPERTY()
	TArray<TObjectPtr<UInventorySlotWidget>> Slots;

	
	// Widgets
	/** */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryPanel;
	

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	// Native
	virtual void SynchronizeProperties() override;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void MakeSlots();
};
using InventoryWidgetPtr = TObjectPtr<UInventoryWidget>;