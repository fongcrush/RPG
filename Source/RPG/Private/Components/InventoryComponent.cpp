// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Items/ItemStackBase.h"
#include "RPG/RPG.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	TotalWeight = 0.f;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

UItemStackBase* UInventoryComponent::FindItemStack(UItemStackBase* Item) const
{
	if (InventoryContents.Contains(Item) && Item)
	{
		return Item;
	}
	return nullptr;
}
 
UItemStackBase* UInventoryComponent::FindNoneFullStack(UItemStackBase* Item) const
{
	if (const TObjectPtr<UItemStackBase>* Result = InventoryContents.FindByPredicate([&Item](const TObjectPtr<UItemStackBase>& InventoryItem)
		{
			return (!InventoryItem->IsFullStack()) && (Item->GetDataReference() == InventoryItem->GetDataReference());
		}))
	{
		return *Result;
	}	
	return nullptr;
}

void UInventoryComponent::RemoveStack(UItemStackBase* Item)
{
	InventoryContents.RemoveSingle(Item);
	TotalWeight -= Item->GetStackWeight();
	
	OnInventoryChanged.Broadcast();
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
		InventoryContents.RemoveSingle(Item);
	}
	
	TotalWeight -= QuantityToRemove * Item->GetSingleWeight();
	
	OnInventoryChanged.Broadcast();
	
	return QuantityToRemove;
}

void UInventoryComponent::SplitStack(UItemStackBase* Item, const int32 Quantity)
{
	RemoveStackQuantity(Item, Quantity);
	AddNewStack(Item, Quantity);
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemStackBase* Item)
{
	if (GetOwner())
	{
		const int32& RequestedQuantity = Item->Quantity;
		if (RequestedQuantity <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString(TEXT("인벤토리에 {0}를 추가할수 없습니다. 잘못된 수량입니다.")),Item->GetDataReference()->Name));
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

FItemAddResult UInventoryComponent::HandleAddStackable(const TObjectPtr<UItemStackBase>& Item, const int32 RequestedQuantity)
{
	const int32 QuantityCanAdd = GetItemQuantityCanAdd(Item, RequestedQuantity);
	const int32 LeftQuantity = RequestedQuantity > QuantityCanAdd ? RequestedQuantity - QuantityCanAdd : 0;
	
	int32 Quantity = QuantityCanAdd;
	while (Quantity)
	{
		// 자리가 남은 기존 스택이 있음 / 없음
		if (UItemStackBase* ExistingItem = FindNoneFullStack(Item))
		{
			const int32 QuantityToAdd = FMath::Min(ExistingItem->GetEmptySize(), Quantity);
			ExistingItem->SetQuantity(ExistingItem->Quantity + QuantityToAdd);
			Quantity -= QuantityToAdd;
			
			OnInventoryChanged.Broadcast();
		}
		else
		{
			const int32 QuantityToAdd = FMath::Min(Item->GetMaxSize(), Quantity);
			AddNewStack(Item, QuantityToAdd);
			Quantity -= QuantityToAdd;
		}
	}
	
	if (LeftQuantity > 0)
	{
		Item->SetQuantity(LeftQuantity);
	}

	// 추가 결과 반환
	if (QuantityCanAdd == RequestedQuantity)
	{
		return FItemAddResult::AddedAll(RequestedQuantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 추가했습니다.")), Item->GetDataReference()->Name, RequestedQuantity));
	}

	if (QuantityCanAdd < RequestedQuantity && QuantityCanAdd > 0)
	{
		return FItemAddResult::AddedPartial(RequestedQuantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 중 {2}개 추가했습니다.")), Item->GetDataReference()->Name, RequestedQuantity, QuantityCanAdd));
	}

	if (QuantityCanAdd <= 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량 한도를 초과합니다.")),Item->GetDataReference()->Name));
	}

	return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 알 수 없는 에러 입니다.")),Item->GetDataReference()->Name));
}

FItemAddResult UInventoryComponent::HandleAddNoneStackable(const TObjectPtr<UItemStackBase>& Item)
{
	// 가방 용량 초과인지 확인
	if (TotalWeight + Item->GetStackWeight() > WeightCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량 한도를 초과합니다.")),Item->GetDataReference()->Name));
	}

	AddNewStack(Item, 1);
	
	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString(
		TEXT("인벤토리에 {0} 를 추가했습니다.")),Item->GetDataReference()->Name));
}

void UInventoryComponent::AddNewStack(const TObjectPtr<UItemStackBase>& Item, const int32 Quantity)
{
	UItemStackBase* NewItem = Item->CreateCopy();

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(Quantity);
	InventoryContents.Add(NewItem);
	
	TotalWeight += NewItem->GetStackWeight();
	
	OnInventoryChanged.Broadcast();
}



int32 UInventoryComponent::GetItemQuantityCanAdd(const TObjectPtr<UItemStackBase>& Item, const int32 Quantity) const
{
	return  FMath::Min(Quantity, FMath::FloorToInt(WeightCapacity - TotalWeight / Item->GetSingleWeight()));
}