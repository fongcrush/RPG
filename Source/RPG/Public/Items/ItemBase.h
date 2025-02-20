// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDataStructs.h"
#include "Interfaces/DynamicItem.h"
#include "RPG/RPG.h"
#include "ItemBase.generated.h"

class UInventorySlotWidget;
class UItemBase;
class AItemActor;

UCLASS(Blueprintable, BlueprintType)
class RPG_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle StaticDataHandle;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓	
	/** 정적/동적 아이템 판별 후 CDO/Instnace 반환 */
	template <typename T>
	static T* GetValidItem(UItemBase* Item);
	static UItemBase* GetValidItem(UItemBase* Item) { return GetValidItem<UItemBase>(Item); }

	/** 정적/동적 아이템 판별 후 CDO/Instnace 반환 */
	template <typename T>
	static T* GetValidItem(UClass* Item);
	static UItemBase* GetValidItem(UClass* Item) { return GetValidItem<UItemBase>(Item); }


	UFUNCTION(Category="Inventory")
	virtual void Drop(AActor* Owner, const int32 QuantityToDrop);

	// Getters
	FORCEINLINE FItemStaticBase* GetStaticData() const { return StaticDataHandle.GetRow<FItemStaticBase>(GetName()); }

	UFUNCTION(Category="Item")
	FORCEINLINE FText GetItemName() const { return GetStaticData()->Name; }

	UFUNCTION(Category="Item")
	virtual FORCEINLINE float GetWeight() const { return GetStaticData()->Weight; }

	UFUNCTION(Category="Item")
	FORCEINLINE int32 GetMaxSize() const { return GetStaticData()->MaxStackSize; }

	UFUNCTION(Category="Item")
	FORCEINLINE bool IsStackable() const { return GetStaticData()->bIsStackable; }

	UFUNCTION(Category="Item")
	virtual void Use(AActor* Owner) {}
};

using ItemPtr = TObjectPtr<UItemBase>;

template <typename T>
T* UItemBase::GetValidItem(UItemBase* Item)
{
	if (!Item)
	{
		LOG_CALLSTACK("Item Is nullptr")
		return nullptr;
	}
	if (Item->Implements<UDynamicItem>())
	{
		return DuplicateObject<T>(Cast<T>(Item), nullptr);
	}
	return Cast<T>(Item);
}

template <typename T>
T* UItemBase::GetValidItem(UClass* Item)
{
	if (!Item)
	{
		LOG_CALLSTACK("Item Is nullptr")
		return nullptr;
	}
	
	T* ItemCDO = Cast<T>(Item->GetDefaultObject());
	if (!ItemCDO)
	{
		return nullptr;
	}	
	if (ItemCDO->Implements<UDynamicItem>())
	{
		return DuplicateObject<T>(ItemCDO, nullptr);
	}
	return Cast<T>(Item->GetDefaultObject());
}
