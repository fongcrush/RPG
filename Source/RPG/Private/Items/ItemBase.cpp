// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

#include "World/ItemActor.h"

UItemBase::UItemBase()
{
}
#if WITH_EDITOR
void UItemBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);
	Initialize();
}
#endif
void UItemBase::PostInitProperties()
{
	UObject::PostInitProperties();
	Initialize();
}

void UItemBase::PostLoad()
{
	UObject::PostLoad();
	Initialize();
}

void UItemBase::Initialize()
{
	StaticData = StaticDataHandle.GetRow<FItemStaticBase>(GetName());
}

void UItemBase::Drop(AActor* Owner, const int32 QuantityToDrop)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FVector SpawnLocation(Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 50.f) + FVector(0.f, 0.f, 50.f));
	const FTransform SpawnTransform(Owner->GetActorRotation(), SpawnLocation);
	
	AItemActor* ItemActor = GetWorld()->SpawnActor<AItemActor>(AItemActor::StaticClass(), SpawnTransform, SpawnParams);
	ItemActor->InitializeDrop(this, QuantityToDrop);
}
