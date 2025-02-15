// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Components/TextBlock.h"
#include "Framework/RPGPlayerController.h"
#include "Items/Inventory.h"

#include "Items/ItemStackBase.h"
#include "RPG/RPG.h"
#include "UIs/MainMenu.h"
#include "UIs/Inventory/InventoryWidget.h"
#include "UIs/RPGHUD.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!Inventories[0] || Inventories.Num() == 0)
	{
		LOG_ERROR("Check Inventories binding!");
	}
}

void UInventoryComponent::SplitStack(UItemStackBase* Item, const int32 Quantity)
{
	RemoveStackQuantity(Item, Quantity);
	AddNewStack(Item, Quantity);
}

float UInventoryComponent::GetTotalWeight() const
{
	int32 TotalWeight = 0;
	for (TInventoryPtr SubInventory : Inventories)
	{
		TotalWeight += SubInventory->TotalWeight;
	}
	return TotalWeight;
}

float UInventoryComponent::GetWeightCapacity() const
{
	float TotalWeightCapacity = 0;
	for (TInventoryPtr SubInventory : Inventories)
	{
		TotalWeightCapacity += StructCast<FBagStaticData>(SubInventory->GetStaticData())->WeightCapacity;
	}
	return TotalWeightCapacity;
}

TArray<UItemStackBase*> UInventoryComponent::GetContents() const
{
	TArray<TItemStackPtr> AllContents;
	for (TInventoryPtr SubInventory : Inventories)
	{
		AllContents.Append(SubInventory->GetContents());
	}
	return AllContents;
}