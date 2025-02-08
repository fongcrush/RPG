// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Items/ItemBase.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* Item) const
{
	if (InventoryContents.Contains(Item) && Item)
	{
		return Item;
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* Item) const
{
	if (Item)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(Item))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* Item) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate(
		[&Item](const UItemBase* InventoryItem)
		{
			return (InventoryItem->ItemDataHandle.RowName == Item->ItemDataHandle.RowName) && !InventoryItem->IsFullStack();
		}))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateQuantityCanAdd(const TObjectPtr<UItemBase>& Item, const int32 Quantity) const
{
	const int32 MaxQuantity = FMath::FloorToInt((WeightCapacity - TotalWeight) / Item->GetItemSingleWeight());
	if (Quantity <= MaxQuantity)
	{
		return Quantity;
	}
	return Quantity;
}

int32 UInventoryComponent::CalculateQuantityForFull(const TObjectPtr<UItemBase>& Item, const int32 Quantity) const
{
	const int32 LeftQuantityForFull = Item->GetDataReference()->MaxStackSize - Item->Quantity;
	return FMath::Min(Quantity, LeftQuantityForFull);
}

void UInventoryComponent::RemoveSingleStack(UItemBase* Item)
{
	InventoryContents.RemoveSingle(Item);
	OnInventoryChanged.Broadcast();
}

int32 UInventoryComponent::RemoveMultipleStack(UItemBase* Item, const int32 Quantity)
{
	const int32 ActualQuantityRemove = FMath::Min(Quantity, Item->Quantity);
	Item->SetQuantity(Item->Quantity - ActualQuantityRemove);
	TotalWeight -= ActualQuantityRemove * Item->GetItemSingleWeight();
	
	OnInventoryChanged.Broadcast();
	
	return ActualQuantityRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* Item, const int32 Quantity)
{
	if (InventoryContents.Num() < SlotsCapacity)
	{
		RemoveMultipleStack(Item, Quantity);
		AddNewItem(Item, Quantity);
	}
}

FItemAddResult UInventoryComponent::HandleNoneStackableItems(const TObjectPtr<UItemBase>& Item, const int32 Quantity)
{
	// 아이템 무게가 올바른지 확인
	if (FMath::IsNearlyZero(Item->GetItemStackWeight()) || Item->GetItemStackWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString(TEXT("인벤토리에 {0}를 추가할수 없습니다. 잘못된 중량입니다.")),Item->GetDataReference()->Name));
	}

	// 아이템 무게가 가방 용량을 초과하는지 확인
	if (TotalWeight + Item->GetItemStackWeight() > WeightCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString(TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량이 초과됩니다.")),Item->GetDataReference()->Name));
	}

	// 아이템 슬롯이 가득 찼는지 확인
	if (InventoryContents.Num() >= SlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString(TEXT("인벤토리에 {0}를 추가할수 없습니다. 슬롯이 가득 찼습니다.")),Item->GetDataReference()->Name));
	}

	AddNewItem(Item, Quantity);
	return FItemAddResult::AddedAll(1, FText::Format(
			FText::FromString(TEXT("인벤토리에 {1} 를 추가했습니다.")), Quantity,Item->GetDataReference()->Name));
}

int32 UInventoryComponent::HandleStackableItems(const TObjectPtr<UItemBase>& Item, const int32 Quantity)
{
	return 0;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* Item)
{
	if (GetOwner())
	{
		const int32 RequestedAddQuantity = Item->Quantity;
		if (!Item->GetDataReference()->bIsStackable)
		{
			return HandleNoneStackableItems(Item, RequestedAddQuantity);
		}

		const int32 StackableQuantityAdded = HandleStackableItems(Item, Item->Quantity);
		if (StackableQuantityAdded == RequestedAddQuantity)
		{
			return FItemAddResult::AddedAll(RequestedAddQuantity, FText::Format(
				FText::FromString(TEXT("인벤토리에 {0} {1}를 추가했습니다.")), RequestedAddQuantity,Item->GetDataReference()->Name));
		}

		if (StackableQuantityAdded < RequestedAddQuantity && StackableQuantityAdded > 0)
		{
			return FItemAddResult::AddedPartial(RequestedAddQuantity, FText::Format(
				FText::FromString(TEXT("인벤토리에 {0} {1}를 추가했습니다.")), RequestedAddQuantity,Item->GetDataReference()->Name));
		}

		if (StackableQuantityAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString(TEXT("인벤토리에 {0}를 추가할수 없습니다. 잘못된 중량입니다.")),Item->GetDataReference()->Name));
		}
	}

	return FItemAddResult::AddedNone(FText::FromString(TEXT("인벤토리에 아이템을 추가할수 없습니다.")));
}

void UInventoryComponent::AddNewItem(TObjectPtr<UItemBase> Item, int32 Quantity)
{
	UItemBase* NewItem;

	// TODO: 아직 정확히 모르겠으니 다 하고 확인하기
	if (Item->bIsCopy || Item->bIsPickup)
	{
		// 이미 월드에 존재하거나 복사본일 때
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// 아이템을 일부 옮겨 담거나 나눌 때
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwingInventory = this;
	NewItem->SetQuantity(Quantity);

	InventoryContents.Add(NewItem);
	TotalWeight += NewItem->GetItemStackWeight();
	OnInventoryChanged.Broadcast();
}