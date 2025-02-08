// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/InventoryPanel.h"

// UE
#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"

// User Defined
#include "Components/WrapBox.h"
#include "Framework/RPGCharacter.h"
#include "UIs/Inventory/InventoryItemSlot.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ARPGCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		Inventory = PlayerCharacter->GetInventory();
		if (Inventory)
		{
			Inventory->OnInventoryChanged.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}
}


void UInventoryPanel::SetInfoText() const
{
	WeightInfo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")),
	                                  Inventory->GetTotalWeight(),
	                                  Inventory->GetWeightCapacity()));
	CapacityInfo->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")),
	                                    Inventory->GetInventoryContents().Num(),
	                                    Inventory->GetSlotsCapacity()));
}

void UInventoryPanel::RefreshInventory()
{
	if (Inventory && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for (UItemBase* const& InventoryItem : Inventory->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}
	}
}


bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
