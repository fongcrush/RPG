// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/InventoryPanel.h"

// UE
#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"

// User Defined
#include "FongcrushUtilities.h"
#include "Components/UniformGridPanel.h"
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

void UInventoryPanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	MakeSlots();
}

void UInventoryPanel::SetInfoText() const
{
	CurrentWeightInfo->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Inventory->GetTotalWeight())));
	CapacityInfo->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Inventory->GetWeightCapacity())));
}

void UInventoryPanel::RefreshInventory()
{
	if (Inventory && InventorySlotClass)
	{
		for (UItemStackBase* const& InventoryItem : Inventory->GetInventoryContents())
		{

		}

		SetInfoText();
	}
}


bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryPanel::MakeSlots()
{
	InventoryPanel->ClearChildren();
	InventorySlots.Empty();
		
	for (int Row = 0; Row < SlotRowSize; ++Row)
	{
		for (int Column = 0; Column < SlotColumnSize; ++Column)
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			InventoryPanel->AddChildToUniformGrid(ItemSlot ,Row, Column);
			InventorySlots.Add(ItemSlot);
		}
	}
}
