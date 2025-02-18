// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemStackBase.h"

#include "World/ItemActor.h"

UItemStackBase::UItemStackBase()
{
}

void UItemStackBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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

void UItemStackBase::DropItem(AActor* Owner, const int32 QuantityToDrop)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FVector SpawnLocation(Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 50.f) + FVector(0.f, 0.f, 50.f));
	const FTransform SpawnTransform(Owner->GetActorRotation(), SpawnLocation);
	
	AItemActor* ItemActor = GetWorld()->SpawnActor<AItemActor>(AItemActor::StaticClass(), SpawnTransform, SpawnParams);
	ItemActor->InitializeDrop(this, QuantityToDrop);

	SetQuantity(Quantity - QuantityToDrop);
}

UItemStackBase* UItemStackBase::Split()
{
	return nullptr;
}

void UItemStackBase::SetQuantity(const int32 NewQuantity)
{
	Quantity = FMath::Clamp(NewQuantity, 0, IsStackable() ? GetMaxSize() : 1);
	if (Quantity == 0)
	{
		ConditionalBeginDestroy();
	}
}
