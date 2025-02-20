// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Items/Inventory.h"
#include "Items/ItemBase.h"
#include "UIs/RPGHUD.h"
#include "UIs/Inventory/InventoryWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::PostInitProperties()
{
	Super::PostInitProperties();
	// 기본 인벤토리 클래스가 설정되지 않았을 경우 기본값 설정
	if (DefaultInventoryClasses.IsEmpty())
	{
		const auto DefaultWidgetPath = TEXT("/Script/Engine.Blueprint'/Game/Item/DefaultInventory.DefaultInventory'");
		UClass* WidgetClass = LoadClass<UInventoryWidget>(nullptr, DefaultWidgetPath);
		DefaultInventoryClasses.Add(WidgetClass);
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// 미리 설정된 인벤토리 생성
	if (const ARPGHUD* HUD = Cast<ARPGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		for (auto& InventoryClass : DefaultInventoryClasses)
		{
			UInventory* Inventory = NewObject<UInventory>(this, InventoryClass);
			Inventory->Posses();
		}
	}
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* const& Item, const int32& Quantity)
{
	// 아이템 개수가 유효한지
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

FItemAddResult UInventoryComponent::HandleAddStackable(UItemBase* const& Item, const int32& Quantity)
{
	const int32 QuantityCanWeight = FMath::Min(Quantity, FMath::FloorToInt(WeightCapacity - GetTotalWeight() / Item->GetWeight()));
	int32 LeftQuantity = QuantityCanWeight;
	HandleAddToExistingStack(Item, LeftQuantity);
	HandleAddToEmptySlot(Item, LeftQuantity);
	
	const FText& ItemName = Item->GetStaticData()->Name;
	// 모두 추가 성공
	if (QuantityCanWeight == Quantity)
	{
		return FItemAddResult::AddedAll(Quantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 추가했습니다.")), ItemName, Quantity));
	}
	// 일부만 추가 성공
	if (QuantityCanWeight < Quantity && QuantityCanWeight > 0)
	{
		return FItemAddResult::AddedPartial(Quantity, LeftQuantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 중 {2}개 추가했습니다.")),ItemName, Quantity, QuantityCanWeight));
	}
	// 중량 초과
	if (QuantityCanWeight <= 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량 한도를 초과합니다.")), ItemName));
	}
	// 알 수 없는 에러
	return FItemAddResult::AddedNone(FText::Format(FText::FromString(
		TEXT("인벤토리에 {0}를 추가할수 없습니다. 알 수 없는 에러 입니다.")), ItemName));
}

void UInventoryComponent::HandleAddToExistingStack(UItemBase* const& Item, int32& LeftQuantity)
{
	int32 InventoryIndex = 0;
	while (InventoryIndex < Inventories.Num())
	{
		const int32 QuantityAdded = Inventories[InventoryIndex]->AddExisting(Item, LeftQuantity);
		if (QuantityAdded != ADD_FAIL)
		{
			LeftQuantity -= QuantityAdded;
			if (LeftQuantity <= 0)
				break;
		}
		else
		{
			++InventoryIndex;
		}
	}
}

void UInventoryComponent::HandleAddToEmptySlot(UItemBase* const& Item, int32& LeftQuantity)
{
	int32 InventoryIndex = 0;
	while (InventoryIndex < Inventories.Num())
	{
		const int32 QuantityAdded = Inventories[InventoryIndex]->AddStack(Item, LeftQuantity);
		if (QuantityAdded != ADD_FAIL)
		{
			LeftQuantity -= QuantityAdded;
			if (LeftQuantity <= 0)
				break;
		}
		else
		{
			++InventoryIndex;
		}
	}
}

FItemAddResult UInventoryComponent::HandleAddNoneStackable(UItemBase* const& Item) const
{
	UInventory* TargetInventory = nullptr;
	for (const InventoryPtr& Inventory : Inventories)
	{
		if (Inventory->FindEmptySlot())
		{
			TargetInventory = Inventory;
			break;
		}
	}
	// 인벤토리가 가득 참
	if (!TargetInventory)
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
	if (TargetInventory->AddStack(Item, 1))
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

TArray<UItemBase*> UInventoryComponent::GetItems() const
{
	TArray<ItemPtr> AllContents;
	for (const InventoryPtr& SubInventory : Inventories)
	{
		AllContents.Append(SubInventory->GetItems());
	}
	return AllContents;
}