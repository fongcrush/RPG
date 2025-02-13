// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/MainMenu.h"

#include "Blueprint/WidgetTree.h"

#include "Components/InventoryComponent.h"
#include "Framework/RPGCharacter.h"
#include "Items/ItemStackBase.h"
#include "UIs/Inventory/InventoryWidget.h"
#include "UIs/Inventory/ItemDragDropOperation.h"

void UMainMenu::InitializeMainInventory()
{
	PlayerCharacter = Cast<ARPGCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		MainInventoryWidget = CreateWidget<UInventoryWidget>(PlayerCharacter, PlayerCharacter->GetInventory()->InventoryWidgetClass);
		
	}
	
}

void UMainMenu::AddSubInventory(UInventoryWidget* SubInventoryWidget)
{
	
}

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenu::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	TArray<UWidget*> ChildWidgets;
	WidgetTree->GetChildWidgets(this, ChildWidgets);
	
	for (UWidget* ChildWidget : ChildWidgets)
	{
		ChildWidget->SynchronizeProperties();
	}
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<ARPGCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDropOperation = Cast<UItemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDropOperation->SourceItem)
	{
		PlayerCharacter->DropItem(ItemDragDropOperation->SourceItem, ItemDragDropOperation->SourceItem->Quantity);
		return true;
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);;
}
