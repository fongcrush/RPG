// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG/RPG.h"
#include "Data/ItemDataStructs.h"
#include "Interfaces/DynamicItem.h"
#include "ItemBase.generated.h"

class UItemSlotWidget;
class UItemBase;
class AItemActor;

UCLASS(Blueprintable, BlueprintType)
class RPG_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
	virtual void PostLoad() override;
	
	/** 정적/동적 아이템 판별 후 CDO/Instnace 반환 */
	template <typename T>
	static T* GetValidItem(UItemBase* const& Item);
	static UItemBase* GetValidItem(UItemBase* const& Item) { return GetValidItem<UItemBase>(Item); }

	/** 정적/동적 아이템 판별 후 CDO/Instnace 반환 */
	template <typename T>
	static T* GetValidItem(UClass* const& Item);
	static UItemBase* GetValidItem(UClass* const& Item) { return GetValidItem<UItemBase>(Item); }


	UFUNCTION(Category="Inventory")
	virtual void Drop(AActor* const& Owner, const int32& QuantityToDrop);

	// Getters
	UFUNCTION(Category="Item")
	FORCEINLINE FDataTableRowHandle GetStaticDataHandle() const { return StaticDataHandle; }
	FORCEINLINE FItemStaticBase* GetStaticData() const { return StaticData; }

	UFUNCTION(Category="Item")
	FORCEINLINE FText GetItemName() const { return GetStaticData()->Name; }

	UFUNCTION(Category="Item")
	virtual float GetWeight() const { return GetStaticData()->Weight; }

	UFUNCTION(Category="Item")
	FORCEINLINE int32 GetMaxSize() const { return IsStackable() ? GetStaticData()->MaxStackSize : 1; }

	UFUNCTION(Category="Item")
	FORCEINLINE bool IsStackable() const { return GetStaticData()->bIsStackable; }

	UFUNCTION(Category="Item")
	virtual void Use(AActor* Owner) {}

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle StaticDataHandle;
	FItemStaticBase* StaticData;
};

template <typename T>
T* UItemBase::GetValidItem(UItemBase* const& Item)
{
	if (!Item)
	{
		LOG_CALLSTACK("Item Is nullptr")
		return nullptr;
	}
	if (Item->Implements<UDynamicItem>())
	{
		return DuplicateObject<T>(Item, nullptr);
	}
	return Cast<T>(Item);
}

template <typename T>
T* UItemBase::GetValidItem(UClass* const& Item)
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

using ItemPtr = TObjectPtr<UItemBase>;