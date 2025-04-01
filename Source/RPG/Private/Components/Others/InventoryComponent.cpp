// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Others/InventoryComponent.h"

#include "Frameworks/RPGHUD.h"
#include "Objects/Items/Inventory.h"
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
		const auto DefaultWidgetPath = TEXT("/Script/Engine.DataTable'/Game/Item/DT_Inventory.DT_Inventory'");
		UClass* WidgetClass = LoadClass<UInventoryWidget>(nullptr, DefaultWidgetPath);
		DefaultInventoryClasses.Add(WidgetClass);
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// 미리 설정한 인벤토리 생성
	if (const ARPGHUD* HUD = Cast<ARPGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		for (auto& InventoryClass : DefaultInventoryClasses)
		{
			UInventory* Inventory = NewObject<UInventory>(this, InventoryClass);
			Inventory->Use(GetOwner());
			Inventories.Add(Inventory);
		}
	}
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* const& Item, const int32& Quantity)
{
	// 쌓을 수 (있는 / 없는) 아이템 처리
	if (Item->IsStackable())
	{
		return HandleAddStackable(Item, Quantity);
	}
	return HandleAddNoneStackable(Item);
}

FItemAddResult UInventoryComponent::HandleAddStackable(UItemBase* const& Item, const int32& Quantity)
{
	FItemStaticBase* ItemStaticData = Item->GetStaticData();
	const FText& ItemName = ItemStaticData->Name;
	const int32 QuantityCanWeight = FMath::Min(Quantity, FMath::FloorToInt(MaxWeight - GetTotalWeight() / ItemStaticData->Weight));
	
	// 중량 초과
	if (QuantityCanWeight <= 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("{0}를 추가할수 없습니다. 중량 한도를 초과합니다.")), ItemName));
	}
	
	// 아이템 추가
	int32 LeftQuantity = HandleAddToExistingStack(Item, QuantityCanWeight);;
	LeftQuantity = HandleAddToEmptySlot(Item, LeftQuantity);
	
	// 모든 아이템 추가 성공
	if (QuantityCanWeight == Quantity)
	{
		return FItemAddResult::AddedAll(Quantity, FText::Format(FText::FromString(
			TEXT("{0}를 {1}개 추가했습니다.")), ItemName, Quantity));
	}
	// 일부 아이템만 추가 성공
	if (QuantityCanWeight < Quantity && QuantityCanWeight > 0)
	{
		return FItemAddResult::AddedPartial(Quantity, LeftQuantity, FText::Format(FText::FromString(
			TEXT("{0}를 {1}개 중 {2}개 추가했습니다.")),ItemName, Quantity, QuantityCanWeight));
	}
	// 알 수 없는 에러
	return FItemAddResult::AddedNone(FText::Format(FText::FromString(
		TEXT("{0}를 추가할수 없습니다. 알 수 없는 에러 입니다.")), ItemName));
}

int32 UInventoryComponent::HandleAddToExistingStack(UItemBase* const& Item, const int32& Quantity)
{
	int32 InventoryIndex = 0;
	int32 LeftQuantity = Quantity;
	while (InventoryIndex < Inventories.Num())
	{
		const int32 QuantityAdded = Inventories[InventoryIndex]->GetInventoryWidget()->AddExisting(Item, LeftQuantity);
		if (QuantityAdded != ADD_FAIL)
		{
			LeftQuantity -= QuantityAdded;
			if (LeftQuantity <= 0) break;
		}
		else
		{
			++InventoryIndex;
		}
	}
	return LeftQuantity;
}

int32 UInventoryComponent::HandleAddToEmptySlot(UItemBase* const& Item, const int32& Quantity)
{
	int32 InventoryIndex = 0;
	int32 LeftQuantity = Quantity;
	while (InventoryIndex < Inventories.Num())
	{
		const int32 QuantityAdded = Inventories[InventoryIndex]->GetInventoryWidget()->AddStack(Item, LeftQuantity);
		if (QuantityAdded != ADD_FAIL)
		{
			LeftQuantity -= QuantityAdded;
			if (LeftQuantity <= 0) break;
		}
		else
		{
			++InventoryIndex;
		}
	}
	return LeftQuantity;
}

FItemAddResult UInventoryComponent::HandleAddNoneStackable(UItemBase* const& Item) const
{
	// 중량 초과
	if (MaxWeight < GetTotalWeight() + Item->GetWeight())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("{0}를 추가할수 없습니다. 너무 무겁습니다.")), Item->GetStaticData()->Name));
	}
	// 자리 있는 인벤토리 찾고 추가
	for (const TObjectPtr<UInventory>& Inventory : Inventories)
	{
		if (Inventory->GetInventoryWidget()->AddStack(Item, 1) != ADD_FAIL)
		{
			return FItemAddResult::AddedAll(1, FText::Format(FText::FromString(
				TEXT("{0} 를 추가했습니다.")), Item->GetStaticData()->Name));
		}
	}
	// 인벤토리가 가득 참
	return FItemAddResult::AddedNone(FText::Format(FText::FromString(
		TEXT("{0}를 추가할수 없습니다. 인벤토리가 가득 찼습니다.")), Item->GetStaticData()->Name));			
}

float UInventoryComponent::GetTotalWeight() const
{
	int32 TotalWeight = 0;
	for (const TObjectPtr<UInventory>& Inventory : Inventories)
	{
		TotalWeight += Inventory->GetWeight();
	}
	return TotalWeight;
}