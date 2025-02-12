// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemStackBase.h"

UItemStackBase::UItemStackBase()
{
}

UItemStackBase* UItemStackBase::CreateCopy()
{
	UItemStackBase* NewItem = NewObject<UItemStackBase>(this);
	NewItem->OwningInventory = OwningInventory;
	NewItem->DataReference = DataReference;
	NewItem->Quantity = Quantity;;
	NewItem->bIsCopy = true;

	return NewItem;
}

void UItemStackBase::SetQuantity(const int32 NewQuantity)
{
	Quantity = FMath::Clamp(NewQuantity, 0, IsStackable() ? GetMaxSize() : 1);
	if (Quantity == 0)
	{
		ConditionalBeginDestroy();
	}
}

void UItemStackBase::Use(ARPGCharacter* Character)
{
	
}
