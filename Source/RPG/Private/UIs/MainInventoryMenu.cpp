// Fill out your copyright notice in the Description page of Project Settings.

// User Defined
#include "UIs/MainInventoryMenu.h"
#include "Framework/RPGCharacter.h"
#include "UIs/Inventory/InventoryWidget.h"
#include "UIs/Inventory/ItemDragDropOperation.h"

// UE
#include "Blueprint/WidgetTree.h"

bool UMainInventoryMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragOperation = Cast<UItemDragDropOperation>(InOperation);
	const SlotWidgetPtr& SourceSlotWidget = ItemDragOperation->SourceSlotWidget;

	ARPGCharacter* PlayerCharacter = Cast<ARPGCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter && SourceSlotWidget)
	{	
		SourceSlotWidget->Drop(SourceSlotWidget->Quantity);
		return true;
	}
	return false;
}
