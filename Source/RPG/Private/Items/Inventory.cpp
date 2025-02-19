// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory.h"

// UE
#include "Blueprint/UserWidget.h"

// User Defined
#include "Components/ScrollBox.h"
#include "RPG/RPG.h"
#include "UIs/MainInventoryMenu.h"
#include "UIs/RPGHUD.h"
#include "UIs/Inventory/InventoryWidget.h"


UInventory::UInventory()
{
}

void UInventory::Initialize()
{
	Super::Initialize();
	InventoryStaticData = StructCast<FInventoryStaticData>(StaticData);
}

void UInventory::Use(ARPGCharacter* Character)
{
	Super::Use(Character);
	if (!InventoryWidget)
	{
		if (!InventoryStaticData)
		{
			LOG_CALLINFO("인벤토리 정적 데이터가 할당되지 않았습니다.");
			return;
		}
		InitializeWidget();
		ReloadItems();
	}
	MainInventoryMenu->InventoryWidgets.AddUnique(InventoryWidget);
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
}

void UInventory::InitializeWidget()
{
	const APlayerController* const& PlayerController = GetWorld()->GetFirstPlayerController<APlayerController>();
	MainInventoryMenu = Cast<ARPGHUD>(PlayerController->GetHUD())->MainInventoryMenu;
	UScrollBox* InventoryPanel = MainInventoryMenu->InventoryPanel;
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryStaticData->InventoryWidgetClass);
	if (InventoryWidget)
	{
		// InventoryWidget->AddToViewport();
		InventoryPanel->AddChild(InventoryWidget);
	}
}

void UInventory::ReloadItems()
{
	ItemSlotMap.Empty();
	for (const SlotWidgetPtr& Slot : InventoryWidget->Slots)
	{
		if (const ItemPtr& Item = Slot->Item)
		{
			ItemSlotMap.Add(Item, Slot);
		}
	}
}

void UInventory::Drop(AActor* Owner, const int32 QuantityToDrop)
{
	Super::Drop(Owner, QuantityToDrop);
	MainInventoryMenu->InventoryWidgets.Remove(InventoryWidget);
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}

UInventorySlotWidget* UInventory::FindSlot(const UItemBase* const& SearchingItem) const
{
	if (UInventorySlotWidget* const* Slot = ItemSlotMap.Find(SearchingItem))
	{
		return *Slot;
	}
	return nullptr;
}

UInventorySlotWidget* UInventory::FindNoneFullSlot(const UItemBase* const& SearchingItem) const
{
	for (const auto& Slot : InventoryWidget->Slots)
	{
		const ItemPtr& ExistingItem = Slot->Item;
		if (Slot->Quantity < ExistingItem->GetMaxSize() && ExistingItem == SearchingItem && ExistingItem)
		{
			return Slot;
		}
	}
	return nullptr;
}

UInventorySlotWidget* UInventory::FindEmptySlot() const
{
	for (const auto& Slot : InventoryWidget->Slots)
	{
		if (!Slot->Item)
		{
			return Slot;
		}
	}
	return nullptr;
}

float UInventory::GetWeight() const
{
	float TotalWeight = 0.f;
	for (auto& ItemAndSlot : ItemSlotMap)
	{
		if (const auto& Slot = ItemAndSlot.Value)
		{
			TotalWeight += Slot->Item->GetWeight() * Slot->Quantity;
		}
	}
	return TotalWeight;
}

void UInventory::SwapItems(UItemBase* ItemA, UItemBase* ItemB) const
{
	UInventorySlotWidget* SlotA = FindSlot(ItemA);
	UInventorySlotWidget* SlotB = FindSlot(ItemB);

	if (ItemA && ItemB && SlotA && SlotB)
	{
		const int32 TempQuantity = SlotA->Quantity;
		SlotA->Quantity = SlotB->Quantity;
		SlotB->Quantity = TempQuantity;
		SlotA->Item = ItemB;
		SlotB->Item = ItemA;
		SlotA->Refresh();
		SlotB->Refresh();
	}
	else
	{
		LOG_CALLINFO_WARNING("인벤토리에 해당 아이템이 없습니다.");
	}
}

TArray<SlotWidgetPtr> UInventory::GetSlots() const
{
	return InventoryWidget->Slots;
}

bool UInventory::AddStack(UItemBase* const& Item, const int32& Quantity)
{
	if (!Item)
	{
		LOG_CALLINFO_WARNING("유효한 아이템이 아닙니다.")
		return false;
	}
	if (UInventorySlotWidget* const& EmptySlot = FindEmptySlot())
	{
		UItemBase* NewItem = DuplicateObject<UItemBase>(Item, this);
		EmptySlot->Item = Item;
		EmptySlot->Quantity = Quantity;
		EmptySlot->Refresh();
		return true;
	}
	LOG_WARNING("인벤토리에 빈 슬롯이 없습니다.");
	return false;
}

bool UInventory::RemoveExisting(UItemBase* Item)
{
	if (UInventorySlotWidget* const& Slot = FindSlot(Item))
	{
		Slot->Reset();
		ItemSlotMap.Remove(Item);

		if (!Item->HasAnyFlags(RF_ClassDefaultObject) && Item->Implements<UDynamicItem>())
		{
			Item->ConditionalBeginDestroy();
		}
		return true;
	}
	LOG_CALLINFO_WARNING("인벤토리에 해당 아이템이 없습니다.");
	return false;
}

int32 UInventory::RemoveQuantity(UItemBase* Item, const int32 Quantity)
{
	if (Quantity <= 0)
	{
		LOG_CALLINFO_WARNING("잘못된 수량입니다.");
		return 0;
	}
	if (!Item)
	{
		LOG_CALLINFO_WARNING("인벤토리에 해당 아이템이 없습니다.");
		return 0;
	}

	const auto TargetSlot = FindSlot(Item);
	if (!TargetSlot)
	{
		LOG_CALLINFO_WARNING("인벤토리에 해당 아이템이 없습니다.");
		return 0;
	}

	const int32 QuantityRemoved = FMath::Min(Quantity, TargetSlot->Quantity);
	TargetSlot->Quantity -= QuantityRemoved;

	if (TargetSlot->Quantity <= 0)
	{
		TargetSlot->Reset();
		ItemSlotMap.Remove(Item);
	}
	return QuantityRemoved;
}
