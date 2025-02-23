// Fill out your copyright notice in the Description page of Project Settings.

#include "UIs/Inventory/InventoryWidget.h"

// UE
#include "Components/UniformGridPanel.h"

// User Defined
#include "RPG/RPG.h"
#include "Interfaces/DynamicItem.h"
#include "Items/ItemBase.h"
#include "UIs/Inventory/ItemSlotWidget.h"


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
	ItemSlots.Empty();
	// 슬롯 생성 및 등록
	for (int Row = 0; Row < SlotRowSize; ++Row)
	{
		for (int Column = 0; Column < SlotColumnSize; ++Column)
		{
			UItemSlotWidget* NewSlot = CreateWidget<UItemSlotWidget>(this, SlotClass);
			InventoryPanel->AddChildToUniformGrid(NewSlot ,Row, Column);
			NewSlot->Index = Row * SlotColumnSize + Column;
			NewSlot->SetParent(this);
			ItemSlots.Add(NewSlot);
		}
	}
}

UItemSlotWidget* UInventoryWidget::FindSlot(const UItemBase* const& SearchingItem) const
{
	for (const auto& ItemSlot : ItemSlots)
	{
		if (ItemSlot->Item == SearchingItem)
		{
			return ItemSlot;
		}
	}
	return nullptr;
}

UItemSlotWidget* UInventoryWidget::FindNoneFullSlot(const UItemBase* const& SearchingItem) const
{
	for (const auto& ItemSlot : ItemSlots)
	{
		if (ItemSlot->Item && !ItemSlot->IsFull() && ItemSlot->Item == SearchingItem)
		{
			return ItemSlot;
		}
	}
	return nullptr;
}

UItemSlotWidget* UInventoryWidget::FindEmptySlot()
{
	for (const auto& ItemSlot : ItemSlots)
	{
		if (!ItemSlot->Item)
		{
			return ItemSlot;
		}
	}
	return nullptr;
}

bool UInventoryWidget::Contains(const UItemBase* const& SearchingItem) const
{
	for (const auto& ItemSlot : ItemSlots)
	{
		if (ItemSlot->Item == SearchingItem)
		{
			return true;
		}
	}
	return false;
}

float UInventoryWidget::GetWeight() const
{
	float TotalWeight = 0.f;
	for (auto& ItemSlot : ItemSlots)
	{
		if (ItemSlot->Item)
		{
			TotalWeight += ItemSlot->Item->GetWeight() * ItemSlot->Quantity;
		}
	}
	return TotalWeight;
}

int32 UInventoryWidget::AddExisting(UItemBase* const& Item, const int32& Quantity)
{
	// 기존 스택 중 자리가 남은 곳 찾기
	if (UItemSlotWidget* const& NoneFullSlot = FindNoneFullSlot(Item))
	{
		const int32 QuantityAdded = FMath::Min(NoneFullSlot->GetEmptySize(), Quantity);
		NoneFullSlot->Quantity += QuantityAdded;
		NoneFullSlot->Refresh();
		return QuantityAdded;
	}
	return ADD_FAIL;
}

int32 UInventoryWidget::AddStack(UItemBase* const& Item, const int32& Quantity)
{
	// 빈 슬롯 찾기
	if (UItemSlotWidget* const& EmptySlot = FindEmptySlot())
	{
		const int32 QuantityAdded = FMath::Min(Quantity, Item->GetMaxSize());
		UItemBase* NewItem = DuplicateObject<UItemBase>(Item, this);
		EmptySlot->Item = Item;
		EmptySlot->Quantity = QuantityAdded;
		EmptySlot->Refresh();
		return QuantityAdded;
	}
	return ADD_FAIL;	
}

bool UInventoryWidget::RemoveExisting(UItemBase* const& Item)
{
	if (UItemSlotWidget* const& ItemSlot = FindSlot(Item))
	{
		ItemSlot->Reset();
		// 동적 아이템(Non-CDO)이면 제거
		if (!Item->HasAnyFlags(RF_ClassDefaultObject) && Item->Implements<UDynamicItem>())
		{
			Item->ConditionalBeginDestroy();
		}
		return true;
	}
	LOG_CALLINFO_WARNING("해당 아이템이 없습니다.");
	return false;
}

int32 UInventoryWidget::RemoveQuantity(UItemBase* const& Item, const int32 Quantity)
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
	}
	return QuantityRemoved;
}

void UInventoryWidget::SwapItems(UItemSlotWidget* const& SlotA, UItemSlotWidget* const& SlotB) const
{
	if ((SlotA->Item || SlotB->Item) && SlotA && SlotB)
	{
		const int32 TempQuantity = SlotA->Quantity;
		SlotA->Quantity = SlotB->Quantity;
		SlotB->Quantity = TempQuantity;
		SlotA->Item = SlotB->Item;
		SlotB->Item = SlotA->Item;
		SlotA->Refresh();
		SlotB->Refresh();
	}
	else
	{
		LOG_CALLINFO_WARNING("인벤토리에 해당 아이템이 없습니다.");
	}
}