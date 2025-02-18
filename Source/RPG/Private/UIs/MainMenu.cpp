// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/MainMenu.h"

#include "Blueprint/WidgetTree.h"
#include "Framework/RPGCharacter.h"
#include "Items/ItemStackBase.h"
#include "UIs/Inventory/InventoryWidget.h"
#include "UIs/Inventory/ItemDragDropOperation.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<ARPGCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragOperation = Cast<UItemDragDropOperation>(InOperation);

	const ItemStackPtr& SourceItemStack = ItemDragOperation->SourceItemStack;
	if (PlayerCharacter && ItemDragOperation->SourceItemStack)
	{
		SourceItemStack->DropItem(PlayerCharacter, SourceItemStack->Quantity);
		ItemDragOperation->SourceSlotWidget->Refresh();
		return true;
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);;
}
