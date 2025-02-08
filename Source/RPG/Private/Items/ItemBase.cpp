// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

#include "Components/InventoryComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

UItemBase::UItemBase()
{
	ResetItemFlags();
	
	bIsLoaded = false;
}

void UItemBase::PostInitProperties()
{
	UObject::PostInitProperties();

	Initialize();
}

void UItemBase::Initialize()
{
	if (ItemDataHandle.DataTable)
	{
		if (FItemDataBase* ItemData = ItemDataHandle.DataTable->FindRow<FItemDataBase>(ItemDataHandle.RowName, ItemDataHandle.RowName.ToString()))
		{
			bIsLoaded = true;
		}
	}
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = DuplicateObject(this, nullptr);
	
	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, DataReference->bIsStackable ? DataReference->MaxStackSize : 1);

		if (OwingInventory)
		{
			if (Quantity <= 0)
			{
				OwingInventory->RemoveSingleStack(this);
			}
		}
	}
}

void UItemBase::Use(ARPGCharacter* Character)
{
	
}
