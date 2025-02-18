// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory.h"

// UE
#include "Blueprint/UserWidget.h"

// User Defined
#include "Components/CanvasPanel.h"
#include "RPG/RPG.h"
#include "UIs/MainMenu.h"
#include "UIs/RPGHUD.h"
#include "Framework/RPGPlayerController.h"
#include "UIs/Inventory/InventoryWidget.h"


UInventory::UInventory()
{
}

void UInventory::Initialize()
{
	Super::Initialize();
	BagStaticData = StructCast<FInventoryStaticData>(StaticData);
}

void UInventory::Use(ARPGCharacter* Character)
{
	Super::Use(Character);
	if (!InventoryWidget)
	{
		if (BagStaticData)
		{
			ARPGPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ARPGPlayerController>();
			UCanvasPanel* MainCanvas = Cast<ARPGHUD>(PlayerController->GetHUD())->MainMenuWidget->MainCanvas;
			InventoryWidget = CreateWidget<UInventoryWidget>(MainCanvas, BagStaticData->InventoryWidgetClass, *(StaticData->Name.ToString()));
			LoadSlots();
		}
		else
		{
			return;
		}
	}

	if (InventoryWidget)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInventory::LoadSlots()
{
	Contents.Empty();
	FilledSlots.Empty();
	EmptySlots.Empty();
	for (const SlotWidgetPtr& Slot : InventoryWidget->Slots)
	{
		if (const ItemStackPtr& ItemStack = Slot->GetItemStack())
		{
			Contents.Add(ItemStack);
			FilledSlots.HeapPush(Slot->Index);
		}
		else
		{
			EmptySlots.HeapPush(Slot->Index);
		}
	}
}

SlotWidgetPtr UInventory::FindNoneFullSlot(const ItemStackPtr& InItemStack) const
{
	for (const int32& Index : FilledSlots)
	{
		const SlotWidgetPtr& Slot = InventoryWidget->Slots[Index];
		const ItemStackPtr& ItemStack = Slot->GetItemStack();
		if (!ItemStack->IsFullStack() && ItemStack->GetStaticData() == InItemStack->GetStaticData())
		{
			return Slot;
		}
	}
	return nullptr;
}

TArray<SlotWidgetPtr> UInventory::GetSlots() const
{
	return InventoryWidget->Slots;
}

void UInventory::AddStack(const TObjectPtr<UItemStackBase>& ItemStack, const int32& InQuantity)
{
	UItemStackBase* NewItem = DuplicateObject<UItemStackBase>(ItemStack, this);
	NewItem->SetQuantity(Quantity);	
	Weight += NewItem->GetStackWeight();
	
	int32 Index = 0;
	Contents.Add(ItemStack);
	EmptySlots.HeapPop(Index);
	FilledSlots.HeapPush(Index);
	InventoryWidget->Slots[Index]->Refresh();
	
	SlotMap[NewItem] = InventoryWidget->Slots[Index];
}

bool UInventory::RemoveExisting(UItemStackBase* ItemStack)
{
	const int32 Index = FilledSlots.IndexOfByKey(FindSlot(ItemStack)->Index);
	if (Index != INDEX_NONE)
	{
		Contents.Remove(ItemStack);
		FilledSlots.HeapRemoveAt(Index);
		EmptySlots.HeapPush(Index);
		SlotMap.Remove(ItemStack);
		InventoryWidget->Slots[Index]->Reset();
		
		// TODO: 총 중량 갱신하기
		
		return true;
	}
	return false;
}

int32 UInventory::RemoveItemQuantity(UItemStackBase* ItemStack, const int32 RequestedQuantity)
{
	if (RequestedQuantity <= 0 || !ItemStack)
	{
		return 0;
	}

	const int32 QuantityToRemove = FMath::Min(RequestedQuantity, ItemStack->Quantity);
	ItemStack->SetQuantity(ItemStack->Quantity - QuantityToRemove);

	if (ItemStack->IsEmpty())
	{
		RemoveExisting(ItemStack);
	}
	else
	{
		FindSlot(ItemStack)->Refresh();
		Weight -= QuantityToRemove * ItemStack->GetSingleWeight();
	}

	return QuantityToRemove;
}

void UInventory::SplitStack(UItemStackBase* Item, const int32 InQuantity)
{
	RemoveItemQuantity(Item, InQuantity);
	AddStack(Item, InQuantity);
}

void UInventory::DropItemQuantity(AActor* Owner, UItemStackBase* DroppingStack, const int32 DroppingQuantity) const
{
	if (Contains(DroppingStack))
	{
		DroppingStack->DropItem(Owner, DroppingQuantity);
		FindSlot(DroppingStack)->Refresh();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to DropItem"));
	}
}