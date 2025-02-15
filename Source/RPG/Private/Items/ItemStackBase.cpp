// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemStackBase.h"

UItemStackBase::UItemStackBase()
{
}

void UItemStackBase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);
	Initialize();
}

void UItemStackBase::PostInitProperties()
{
	UObject::PostInitProperties();
	Initialize();
}

void UItemStackBase::PostLoad()
{
	UObject::PostLoad();
	Initialize();
}

void UItemStackBase::Initialize()
{
	StaticData = StaticDataHandle.GetRow<FItemStaticBase>(GetName());
	if (StaticData)
	{
		Quantity = StaticData->bIsStackable ? FMath::Clamp(Quantity, 0, GetMaxSize()) : 1;
	}
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
