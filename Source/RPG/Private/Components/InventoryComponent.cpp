// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Components/TextBlock.h"

#include "Items/ItemStackBase.h"
#include "UIs/Inventory/InventoryWidget.h"
#include "UIs/RPGHUD.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Weight = 0.f;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ARPGHUD* HUD = Cast<ARPGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	InventoryWidget = CreateWidget<UInventoryWidget>(HUD, InventoryWidgetClass, TEXT("InventoryWidget"));
	RegisterSlots();
}

void UInventoryComponent::Register()
{
	
}

UItemStackBase* UInventoryComponent::FindNoneFullStack(const UItemStackBase* InItemStack) const
{
	if (InventoryMap.Contains(InItemStack))
	{
		for (int32 Index : FilledSlots)
		{
			const TObjectPtr<UItemStackBase>& ItemStack = InventoryWidget->Slots[Index]->GetItemReference();
			if (!ItemStack->IsFullStack() && ItemStack->GetDataReference() == InItemStack->GetDataReference())
			{
				return ItemStack;
			}
		}
	}
	return nullptr;
}

void UInventoryComponent::RemoveStack(UItemStackBase* ItemStack)
{
	UItemSlotWidget* Slot = InventoryMap[ItemStack];
	FilledSlots.HeapRemoveAt(FilledSlots.IndexOfByKey(Slot->Index));
	EmptySlots.HeapPush(Slot->Index);
	Slot->Reset();

	InventoryMap.Remove(ItemStack);

	SetWeight(Weight - ItemStack->GetStackWeight());
}

int32 UInventoryComponent::RemoveStackQuantity(UItemStackBase* Item, const int32 RequestedQuantity)
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
		SetWeight(Weight - QuantityToRemove * Item->GetSingleWeight());
		FindSlot(Item)->Refresh();
	}


	return QuantityToRemove;
}

void UInventoryComponent::SplitStack(UItemStackBase* Item, const int32 Quantity)
{
	RemoveStackQuantity(Item, Quantity);
	AddNewStack(Item, Quantity);
}

TArray<UItemStackBase*> UInventoryComponent::GetContents() const
{
	TArray<TObjectPtr<UItemStackBase>> InventoryContents;
	InventoryMap.GetKeys(InventoryContents);
	return InventoryContents;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemStackBase* Item)
{
	if (GetOwner())
	{
		const int32& RequestedQuantity = Item->Quantity;
		if (RequestedQuantity <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString(
				TEXT("인벤토리에 {0}를 추가할수 없습니다. 잘못된 수량입니다.")), Item->GetDataReference()->Name));
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

FItemAddResult UInventoryComponent::HandleAddStackable(const TObjectPtr<UItemStackBase>& ItemStack, const int32 RequestedQuantity)
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
			TEXT("인벤토리에 {0}를 {1}개 추가했습니다.")), ItemStack->GetDataReference()->Name, RequestedQuantity));
	}

	if (QuantityCanAdd < RequestedQuantity && QuantityCanAdd > 0)
	{
		return FItemAddResult::AddedPartial(RequestedQuantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 중 {2}개 추가했습니다.")),ItemStack->GetDataReference()->Name, RequestedQuantity, QuantityCanAdd));
	}

	if (QuantityCanAdd <= 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량 한도를 초과합니다.")), ItemStack->GetDataReference()->Name));
	}

	return FItemAddResult::AddedNone(FText::Format(FText::FromString(
		TEXT("인벤토리에 {0}를 추가할수 없습니다. 알 수 없는 에러 입니다.")), ItemStack->GetDataReference()->Name));
}

FItemAddResult UInventoryComponent::HandleAddNoneStackable(const TObjectPtr<UItemStackBase>& Item)
{
	// 가방 용량 초과인지 확인
	if (Weight + Item->GetStackWeight() > WeightCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량 한도를 초과합니다.")), Item->GetDataReference()->Name));
	}

	AddNewStack(Item, 1);

	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString(
		TEXT("인벤토리에 {0} 를 추가했습니다.")), Item->GetDataReference()->Name));
}

void UInventoryComponent::AddNewStack(const TObjectPtr<UItemStackBase>& ItemStack, const int32 Quantity)
{
	UItemStackBase* NewItem = ItemStack->CreateCopy();

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(Quantity);

	SetWeight(Weight - NewItem->GetStackWeight());

	int32 Index = 0;
	EmptySlots.HeapPop(Index);
	FilledSlots.HeapPush(Index);
	InventoryWidget->Slots[Index]->Refresh();

	InventoryMap[NewItem] = InventoryWidget->Slots[Index];
}

void UInventoryComponent::SetWeight(const float& InWeight)
{
	Weight = InWeight;
	InventoryWidget->WeightText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Weight)));
	InventoryWidget->CapacityText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Weight)));
}


int32 UInventoryComponent::GetItemQuantityCanAdd(const TObjectPtr<UItemStackBase>& Item, const int32 Quantity) const
{
	return FMath::Min(Quantity, FMath::FloorToInt(WeightCapacity - Weight / Item->GetSingleWeight()));
}

void UInventoryComponent::RegisterSlots()
{
	if (InventoryWidget == nullptr)
	{
		return;
	}

	InventoryMap.Empty();
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