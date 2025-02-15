// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory.h"

// UE
#include "Blueprint/UserWidget.h"

// User Defined
#include "Components/InventoryComponent.h"
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
	BagStaticData = StructCast<FBagStaticData>(StaticData);
}

void UInventory::Use(ARPGCharacter* Character)
{
	Super::Use(Character);
	if (!InventoryWidget)
	{
		if (FBagStaticData* StaticData = StaticDataHandle.GetRow<FBagStaticData>(GetName()))
		{
			ARPGPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ARPGPlayerController>();
			UCanvas* MainCanvas = Cast<ARPGHUD>(PlayerController->GetHUD())->MainMenuWidget->MainCanvas;
			InventoryWidget = CreateWidget<UInventoryWidget>(MainCanvas, StaticData->InventoryWidgetClass, *(StaticData->Name.ToString()));
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

void UInventory::RegisterInventory(UInventoryComponent* Inventory)
{
	
}

void UInventory::LoadSlots()
{
	InventoryMap.Empty();
	FilledSlots.Empty();
	EmptySlots.Empty();
	for (const TObjectPtr<UItemSlotWidget>& Slot : InventoryWidget->Slots)
	{
		if (Slot->GetItemReference())
		{
			InventoryMap[Slot->GetItemReference()] = Slot;
			FilledSlots.HeapPush(Slot->Index);
		}
		else
		{
			EmptySlots.HeapPush(Slot->Index);
		}
	}
}

UItemStackBase* UInventory::FindNoneFullStack(const UItemStackBase* InItemStack) const
{
	if (InventoryMap.Contains(InItemStack))
	{
		for (int32 Index : FilledSlots)
		{
			const TObjectPtr<UItemStackBase>& ItemStack = InventoryWidget->Slots[Index]->GetItemReference();
			if (!ItemStack->IsFullStack() && ItemStack->GetStaticData() == InItemStack->GetStaticData())
			{
				return ItemStack;
			}
		}
	}
	return nullptr;
}


FItemAddResult UInventory::HandleAddItem(UItemStackBase* Item)
{
	if (GetOuter())
	{
		const int32& RequestedQuantity = Item->Quantity;
		if (RequestedQuantity <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString(
				TEXT("인벤토리에 {0}를 추가할수 없습니다. 잘못된 수량입니다.")), Item->GetStaticData()->Name));
		}

		// 쌓을 수 (있는 / 없는) 아이템 처리
		if (Item->IsStackable())
		{
			return HandleAddStackable(Item, RequestedQuantity);
		}
		return HandleAddNoneStackable(Item);
	}

	return FItemAddResult::AddedNone(FText::FromString(TEXT("인벤토리에 아이템을 추가할수 없습니다.")));
}

FItemAddResult UInventory::HandleAddNoneStackable(const TObjectPtr<UItemStackBase>& Item)
{
	// 가방 용량 초과인지 확인
	if (TotalWeight + Item->GetStackWeight() > BagStaticData->WeightCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량 한도를 초과합니다.")), Item->GetStaticData()->Name));
	}

	AddNewStack(Item, 1);

	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString(
		TEXT("인벤토리에 {0} 를 추가했습니다.")), Item->GetStaticData()->Name));
}

FItemAddResult UInventory::HandleAddStackable(const TObjectPtr<UItemStackBase>& ItemStack, const int32 RequestedQuantity)
{
	const int32 QuantityCanAdd = GetItemQuantityCanAdd(ItemStack, RequestedQuantity);
	const int32 LeftQuantity = RequestedQuantity > QuantityCanAdd ? RequestedQuantity - QuantityCanAdd : 0;

	int32 Quantity = QuantityCanAdd;
	while (Quantity)
	{
		// 자리가 남은 기존 스택이 있음 / 없음
		if (UItemStackBase* ExistingItem = FindNoneFullStack(ItemStack))
		{
			const int32 QuantityToAdd = FMath::Min(ExistingItem->GetEmptySize(), Quantity);
			ExistingItem->SetQuantity(ExistingItem->Quantity + QuantityToAdd);
			FindSlot(ExistingItem)->Refresh();
			Quantity -= QuantityToAdd;
		}
		else
		{
			const int32 QuantityToAdd = FMath::Min(ItemStack->GetMaxSize(), Quantity);
			AddNewStack(ItemStack, QuantityToAdd);
			Quantity -= QuantityToAdd;
		}
	}

	if (LeftQuantity > 0)
	{
		ItemStack->SetQuantity(LeftQuantity);
	}

	// 추가 결과 반환
	if (QuantityCanAdd == RequestedQuantity)
	{
		return FItemAddResult::AddedAll(RequestedQuantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 추가했습니다.")), ItemStack->GetStaticData()->Name, RequestedQuantity));
	}

	if (QuantityCanAdd < RequestedQuantity && QuantityCanAdd > 0)
	{
		return FItemAddResult::AddedPartial(RequestedQuantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 중 {2}개 추가했습니다.")),ItemStack->GetStaticData()->Name, RequestedQuantity, QuantityCanAdd));
	}

	if (QuantityCanAdd <= 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량 한도를 초과합니다.")), ItemStack->GetStaticData()->Name));
	}

	return FItemAddResult::AddedNone(FText::Format(FText::FromString(
		TEXT("인벤토리에 {0}를 추가할수 없습니다. 알 수 없는 에러 입니다.")), ItemStack->GetStaticData()->Name));
}

void UInventory::AddNewStack(const TObjectPtr<UItemStackBase>& ItemStack, int32 Quantity)
{
	UItemStackBase* NewItem = DuplicateObject<UItemStackBase>(ItemStack, this);

	NewItem->OwningInventory = Cast<UInventoryComponent>(GetOuter());
	NewItem->SetQuantity(Quantity);

	SetWeight(TotalWeight - NewItem->GetStackWeight());

	int32 Index = 0;
	EmptySlots.HeapPop(Index);
	FilledSlots.HeapPush(Index);
	InventoryWidget->Slots[Index]->Refresh();

	InventoryMap[NewItem] = InventoryWidget->Slots[Index];
}

void UInventory::RemoveStack(UItemStackBase* ItemStack)
{
	UItemSlotWidget* Slot = InventoryMap[ItemStack];
	FilledSlots.HeapRemoveAt(FilledSlots.IndexOfByKey(Slot->Index));
	EmptySlots.HeapPush(Slot->Index);
	Slot->Reset();

	InventoryMap.Remove(ItemStack);

	SetWeight(TotalWeight - ItemStack->GetStackWeight());
}

int32 UInventory::RemoveStackQuantity(UItemStackBase* Item, const int32 RequestedQuantity)
{
	if (RequestedQuantity <= 0 || !Item)
	{
		return 0;
	}

	const int32 QuantityToRemove = FMath::Min(RequestedQuantity, Item->Quantity);
	Item->SetQuantity(Item->Quantity - QuantityToRemove);

	if (Item->IsEmpty())
	{
		RemoveStack(Item);
	}
	else
	{
		SetWeight(TotalWeight - QuantityToRemove * Item->GetSingleWeight());
		FindSlot(Item)->Refresh();
	}

	return QuantityToRemove;
}

void UInventory::SplitStack(UItemStackBase* Item, const int32 Quantity)
{
	RemoveStackQuantity(Item, Quantity);
	AddNewStack(Item, Quantity);
}

void UInventory::SetWeight(const float& InWeight)
{
}

int32 UInventory::GetItemQuantityCanAdd(const TObjectPtr<UItemStackBase>& Item, int32 Quantity) const
{
	return FMath::Min(Quantity, FMath::FloorToInt(BagStaticData->WeightCapacity - TotalWeight / Item->GetSingleWeight()));
}
