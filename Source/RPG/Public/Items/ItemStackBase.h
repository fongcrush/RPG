// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDataStructs.h"
#include "ItemStackBase.generated.h"

class UItemStackBase;
class AItemActor;
class UInventoryComponent;

UCLASS()
class RPG_API UItemStackBase : public UObject
{
	GENERATED_BODY()

	friend AItemActor;
	
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle StaticDataHandle;
	
	UPROPERTY(VisibleInstanceOnly)
	int32 Quantity;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UItemStackBase();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	virtual void Initialize();
	
	UFUNCTION(Category = "Inventory")
	void DropItem(AActor* Owner, const int32 QuantityToDrop);

	UItemStackBase* Split();

	// Getters
	FORCEINLINE FItemStaticBase* GetStaticData() const { return StaticData; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE FText GetItemName() const { return StaticData->Name; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetStackWeight() const { return Quantity * StaticData->Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetSingleWeight() const { return StaticData->Weight; }

	UFUNCTION(Category = "Item")
	FORCEINLINE int32 GetMaxSize() const { return StaticData->MaxStackSize; }

	UFUNCTION(Category = "Item")
	FORCEINLINE int32 GetEmptySize() const { return StaticData->MaxStackSize - Quantity; }
		
	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullStack() const { return IsStackable() ? (Quantity >= StaticData->MaxStackSize) : (Quantity > 0); }

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsStackable() const { return StaticData->bIsStackable; }

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsEmpty() const { return Quantity == 0; }

	/* 아이템의 수량을 설정하고, 수량이 0이 되면 아이템을 제거 */
	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(class ARPGCharacter* Character) {}
	
protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	FItemStaticBase* StaticData;
};
using ItemStackPtr = TObjectPtr<UItemStackBase>;