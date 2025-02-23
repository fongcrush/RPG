// Fill out your copyright notice in the Description page of Project Settings.

// User Defined
#include "UIs/InventoryMenu.h"
#include "Framework/RPGCharacter.h"
#include "UIs/Inventory/ItemDragDropOperation.h"

// UE
#include "Blueprint/WidgetTree.h"
#include "UIs/Inventory/ItemSlotWidget.h"

bool UInventoryMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UItemDragDropOperation* const& ItemDragOperation = Cast<UItemDragDropOperation>(InOperation);
	UItemSlotWidget* const& SourceSlotWidget = ItemDragOperation->SourceSlot;

	ARPGCharacter* PlayerCharacter = Cast<ARPGCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter && SourceSlotWidget)
	{	
		SourceSlotWidget->Drop(SourceSlotWidget->Quantity);
		return true;
	}
	return false;
}
