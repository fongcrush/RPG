// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "World/ItemActor.h"

void UItemBase::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);	
	StaticData = StaticDataHandle.GetRow<FItemStaticBase>(GetName());
	if (!StaticData)
	{
		LOG_CALLINFO_ERROR("%s의 StaticDataHandle 바인딩이 올바르지 않습니다. %s", *GetName(), *(StaticDataHandle.ToDebugString()))
	}
	else
	{
		LOG_CALLINFO("%s %s", *GetName(), *(StaticDataHandle.ToDebugString()))
	}
}

void UItemBase::PostLoad()
{
	Super::PostLoad();
	StaticData = StaticDataHandle.GetRow<FItemStaticBase>(GetName());
	if (!StaticData)
	{
		LOG_CALLINFO_ERROR("%s의 StaticDataHandle 바인딩이 올바르지 않습니다. %s", *GetName(), *(StaticDataHandle.ToDebugString()))
	}
	else
	{
		LOG_CALLINFO("%s %s", *GetName(), *(StaticDataHandle.ToDebugString()))
	}
}

void UItemBase::Drop(AActor* const& Owner, const int32& QuantityToDrop)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FVector SpawnLocation(Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 50.f) + FVector(0.f, 0.f, 50.f));
	const FTransform SpawnTransform(Owner->GetActorRotation(), SpawnLocation);
	
	AItemActor* ItemActor = Owner->GetWorld()->SpawnActor<AItemActor>(AItemActor::StaticClass(), SpawnTransform, SpawnParams);
	ItemActor->InitializeDrop(this, QuantityToDrop);
}
