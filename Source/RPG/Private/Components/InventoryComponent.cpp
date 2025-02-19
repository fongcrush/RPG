// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "RPG/RPG.h"
#include "Items/Inventory.h"
#include "Items/ItemBase.h"
#include "UIs/MainInventoryMenu.h"
#include "UIs/RPGHUD.h"
#include "UIs/Inventory/InventoryWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::PostInitProperties()
{
	Super::PostInitProperties();
	if (Inventories.IsEmpty())
	{
		// TODO: 디폴트 메인 인벤토리 바인딩
		// Inventories.Add()
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (const ARPGHUD* HUD = Cast<ARPGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		HUD->MainInventoryMenu->InventoryWidgets.Add(Inventories[0]->GetInventoryWidget());
	}
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* Item, const int32& Quantity)
{
	if (Quantity <= 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 잘못된 수량입니다.")), Item->GetStaticData()->Name));
	}
	
	// 쌓을 수 (있는 / 없는) 아이템 처리
	if (Item->IsStackable())
	{
		return HandleAddStackable(Item, Quantity);
	}
	return HandleAddNoneStackable(Item);
}

FItemAddResult UInventoryComponent::HandleAddStackable(UItemBase* Item, const int32 Quantity)
{
	const int32 QuantityCanWeight = FMath::Min(Quantity, FMath::FloorToInt(WeightCapacity - GetTotalWeight() / Item->GetWeight()));

	// 기존 스택 중 자리가 남은 스택부터 채우기
	int32 InventoryIndex = 0;
	int32 LeftQuantity = QuantityCanWeight;
	while (InventoryIndex < Inventories.Num())
	{
		if (UInventorySlotWidget* const& NoneFullSlot = Inventories[InventoryIndex]->FindNoneFullSlot(Item))
		{
			const int32 QuantityAdded = FMath::Min(NoneFullSlot->GetEmptySize(), LeftQuantity);
			NoneFullSlot->Quantity += QuantityAdded;
			NoneFullSlot->Refresh();
			
			LeftQuantity -= QuantityAdded;
		}
		else
		{
			++InventoryIndex;
		}
	}
	// 빈 슬롯에 나머지 추가
	InventoryIndex = 0;
	while (InventoryIndex < Inventories.Num())
	{
		int32 NewItemQuantity = FMath::Min(LeftQuantity, Item->GetMaxSize());
		if (Inventories[InventoryIndex]->AddStack(Item, NewItemQuantity))
		{
			LeftQuantity -= NewItemQuantity;
			if (LeftQuantity <= 0)
			{
				break;
			}
		}
		else
		{
			++InventoryIndex;
		}
	}
	// 모두 추가 성공
	if (QuantityCanWeight == Quantity)
	{
		return FItemAddResult::AddedAll(Quantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 추가했습니다.")), Item->GetStaticData()->Name, Quantity));
	}
	// 일부만 추가 성공
	if (QuantityCanWeight < Quantity && QuantityCanWeight > 0)
	{
		return FItemAddResult::AddedPartial(Quantity, LeftQuantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 중 {2}개 추가했습니다.")),Item->GetStaticData()->Name, Quantity, QuantityCanWeight));
	}
	// 중량 초과
	if (QuantityCanWeight <= 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량 한도를 초과합니다.")), Item->GetStaticData()->Name));
	}
	// 알 수 없는 에러
	return FItemAddResult::AddedNone(FText::Format(FText::FromString(
		TEXT("인벤토리에 {0}를 추가할수 없습니다. 알 수 없는 에러 입니다.")), Item->GetStaticData()->Name));
}

FItemAddResult UInventoryComponent::HandleAddNoneStackable(UItemBase* Item) const
{
	UInventory* CurrentInventory = nullptr;
	for (const InventoryPtr& Inventory : Inventories)
	{
		if (Inventory->FindEmptySlot())
		{
			CurrentInventory = Inventory;
			break;
		}
	}
	// 인벤토리가 가득 참
	if (!CurrentInventory)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 인벤토리가 가득 찼습니다.")), Item->GetStaticData()->Name));
	}
	// 중량 초과
	if (WeightCapacity < GetTotalWeight() + Item->GetWeight())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 너무 무겁습니다.")), Item->GetStaticData()->Name));
	}
	// 아이템 추가
	if (CurrentInventory->AddStack(Item, 1))
	{
		return FItemAddResult::AddedAll(1, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0} 를 추가했습니다.")), Item->GetStaticData()->Name));
	}
	// 알 수 없는 에러
	return FItemAddResult::AddedNone(FText::Format(FText::FromString(
		TEXT("인벤토리에 {0}를 추가할수 없습니다. 알 수 없는 에러 입니다.")), Item->GetStaticData()->Name));
}

float UInventoryComponent::GetTotalWeight() const
{
	int32 TotalWeight = 0;
	for (const InventoryPtr& SubInventory : Inventories)
	{
		TotalWeight += SubInventory->GetWeight();
	}
	return TotalWeight;
}

TArray<UItemBase*> UInventoryComponent::GetContents() const
{
	TArray<ItemPtr> AllContents;
	for (const InventoryPtr& SubInventory : Inventories)
	{
		AllContents.Append(SubInventory->GetItems());
	}
	return AllContents;
}