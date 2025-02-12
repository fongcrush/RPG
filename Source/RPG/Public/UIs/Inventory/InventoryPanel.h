// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIs/MovingUserWidget.h"
#include "InventoryPanel.generated.h"


class UUniformGridPanel;
class UInventoryItemSlot;
class UInventoryComponent;
class ARPGCharacter;
class UTextBlock;
class UWrapBox;
/**
 * 
 */
UCLASS()
class RPG_API UInventoryPanel : public UMovingUserWidget
{
	GENERATED_BODY()

public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryPanel;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CurrentWeightInfo;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CapacityInfo;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 SlotColumnSize;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 SlotRowSize;
	
	UPROPERTY()
	TObjectPtr<ARPGCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> Inventory;
	
	UPROPERTY()
	TArray<TObjectPtr<UInventoryItemSlot>> InventorySlots;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UFUNCTION()
	void RefreshInventory();
	
protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	
	virtual void NativeOnInitialized() override;
	virtual void SynchronizeProperties() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void MakeSlots();
	
	void SetInfoText() const;
};
