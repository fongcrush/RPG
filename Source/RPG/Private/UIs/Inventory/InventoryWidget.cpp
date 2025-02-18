// Fill out your copyright notice in the Description page of Project Settings.

// User Defined
#include "UIs/Inventory/InventoryWidget.h"
#include "UIs/Inventory/InventorySlotWidget.h"


void UInventoryWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	MakeSlots();
}

void UInventoryWidget::MakeSlots()
{
	if (!InventoryPanel || !SlotClass)
	{
		return;
	}
	
	InventoryPanel->ClearChildren();
	Slots.Empty();
	
	for (int Row = 0; Row < SlotRowSize; ++Row)
	{
		for (int Column = 0; Column < SlotColumnSize; ++Column)
		{
			UInventorySlotWidget* Slot = CreateWidget<UInventorySlotWidget>(this, SlotClass);
			InventoryPanel->AddChildToUniformGrid(Slot ,Row, Column);
			Slot->Index = Row * SlotColumnSize + Column;
			Slots.Add(Slot);
		}
	}
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}