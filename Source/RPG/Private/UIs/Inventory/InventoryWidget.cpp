// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/InventoryWidget.h"

// User Defined
#include "UIs/Inventory/ItemSlotWidget.h"

void UInventoryWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	MakeSlots();
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryWidget::MakeSlots()
{
	InventoryPanel->ClearChildren();
	Slots.Empty();
		
	for (int Row = 0; Row < SlotRowSize; ++Row)
	{
		for (int Column = 0; Column < SlotColumnSize; ++Column)
		{
			UItemSlotWidget* ItemSlot = CreateWidget<UItemSlotWidget>(this, InventorySlotClass);
			InventoryPanel->AddChildToUniformGrid(ItemSlot ,Row, Column);
			ItemSlot->Index = Row * SlotColumnSize + Column;
			Slots.Add(ItemSlot);
		}
	}
}
