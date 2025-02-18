// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "RPG/RPG.h"
#include "Items/Inventory.h"
#include "Items/ItemStackBase.h"
#include "UIs/MainMenu.h"
#include "UIs/RPGHUD.h"
#include "UIs/Inventory/InventoryWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (const ARPGHUD* HUD = Cast<ARPGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		UInventory* MainInventory = NewObject<UInventory>(this);
		MainInventory->InventoryWidget = HUD->MainMenuWidget->MainInventoryWidget;
		MainInventory->StaticDataHandle.DataTable = LoadClass 
		MainInventory->LoadSlots();
		Inventories.Add(MainInventory);
	}
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemStackBase* Stack)
{
	const int32& RequestedQuantity = Stack->Quantity;
	if (RequestedQuantity <= 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 잘못된 수량입니다.")), Stack->GetStaticData()->Name));
	}
	
	// 쌓을 수 (있는 / 없는) 아이템 처리
	if (Stack->IsStackable())
	{
		return HandleAddStackable(Stack, RequestedQuantity);
	}
	return HandleAddNoneStackable(Stack);
}

FItemAddResult UInventoryComponent::HandleAddNoneStackable(UItemStackBase* Stack) const
{
	UInventory* CurrentInventory = nullptr;
	for (const InventoryPtr& Inventory : Inventories)
	{
		if (Inventory->GetEmptySlots().Num() > 0)
		{
			CurrentInventory = Inventory;
			break;
		}
	}
	if (!CurrentInventory)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 인벤토리가 가득 찼습니다.")), Stack->GetStaticData()->Name));
	}
	
	// 가방 용량 초과인지 확인
	if (WeightCapacity < GetTotalWeight() + Stack->GetStackWeight())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량 한도를 초과합니다.")), Stack->GetStaticData()->Name));
	}

	CurrentInventory->AddStack(Stack, 1);

	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString(
		TEXT("인벤토리에 {0} 를 추가했습니다.")), Stack->GetStaticData()->Name));
}

FItemAddResult UInventoryComponent::HandleAddStackable(UItemStackBase* Stack, const int32 RequestedQuantity)
{
	const int32 QuantityCanAdd = FMath::Min(RequestedQuantity, FMath::FloorToInt(WeightCapacity - GetTotalWeight() / Stack->GetSingleWeight()));

	// 기존 스택 중 자리가 남은 스택부터 채우기
	int32 InventoryIndex = 0;
	int32 LeftQuantity = QuantityCanAdd;
	while (InventoryIndex < Inventories.Num())
	{		
		if (SlotWidgetPtr NoneFullSlot = Inventories[InventoryIndex]->FindNoneFullSlot(Stack))
		{
			UItemStackBase* NoneFullStack = NoneFullSlot->GetItemStack();
			
			const int32 QuantityAdded = FMath::Min(NoneFullStack->GetEmptySize(), LeftQuantity);
			NoneFullStack->SetQuantity(NoneFullStack->Quantity + QuantityAdded);
			NoneFullSlot->Refresh();
			
			Inventories[InventoryIndex]->Weight += QuantityAdded * NoneFullStack->GetSingleWeight();
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
		if (Inventories[InventoryIndex]->GetEmptySlots().Num() > 0)
		{
			int32 NewStackQuantity = FMath::Min(LeftQuantity, Stack->GetMaxSize());
			Inventories[InventoryIndex]->AddStack(Stack, NewStackQuantity);
			LeftQuantity -= NewStackQuantity;
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

	Stack->SetQuantity(LeftQuantity);
	
	// 추가 결과 반환
	if (QuantityCanAdd == RequestedQuantity)
	{
		return FItemAddResult::AddedAll(RequestedQuantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 추가했습니다.")), Stack->GetStaticData()->Name, RequestedQuantity));
	}

	if (QuantityCanAdd < RequestedQuantity && QuantityCanAdd > 0)
	{
		return FItemAddResult::AddedPartial(RequestedQuantity, FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 {1}개 중 {2}개 추가했습니다.")),Stack->GetStaticData()->Name, RequestedQuantity, QuantityCanAdd));
	}

	if (QuantityCanAdd <= 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			TEXT("인벤토리에 {0}를 추가할수 없습니다. 중량 한도를 초과합니다.")), Stack->GetStaticData()->Name));
	}

	return FItemAddResult::AddedNone(FText::Format(FText::FromString(
		TEXT("인벤토리에 {0}를 추가할수 없습니다. 알 수 없는 에러 입니다.")), Stack->GetStaticData()->Name));
}

float UInventoryComponent::GetTotalWeight() const
{
	int32 TotalWeight = 0;
	for (const InventoryPtr& SubInventory : Inventories)
	{
		TotalWeight += SubInventory->Weight;
	}
	return TotalWeight;
}

TArray<UItemStackBase*> UInventoryComponent::GetContents() const
{
	TArray<ItemStackPtr> AllContents;
	for (const InventoryPtr& SubInventory : Inventories)
	{
		AllContents.Append(SubInventory->GetContents());
	}
	return AllContents;
}