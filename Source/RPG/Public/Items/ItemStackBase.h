// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDataStructs.h"
#include "ItemStackBase.generated.h"

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
	UPROPERTY()
	UInventoryComponent* OwningInventory;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;
	
	bool bIsCopy, bIsPickup;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UItemStackBase();

	/* DuplicateObject는 UPROPERTY() 선언된 맴버만 복사하므로 수동 복사 */
	UItemStackBase* CreateCopy();

	FORCEINLINE FItemDataBase* GetDataReference() const { return DataReference; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE FText GetItemName() const { return DataReference->Name; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetStackWeight() const { return Quantity * DataReference->Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetSingleWeight() const { return DataReference->Weight; }

	UFUNCTION(Category = "Item")
	FORCEINLINE int32 GetMaxSize() const { return DataReference->MaxStackSize; }

	UFUNCTION(Category = "Item")
	FORCEINLINE int32 GetEmptySize() const { return DataReference->MaxStackSize - Quantity; }
		
	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullStack() const { return IsStackable() ? (Quantity >= DataReference->MaxStackSize) : (Quantity > 0); }

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsStackable() const { return DataReference->bIsStackable; }

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsEmpty() const { return Quantity == 0; }

	/* 아이템의 수량을 설정하고, 수량이 0이 되면 아이템을 제거 */
	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(class ARPGCharacter* Character);

	bool operator==(const FItemDataBase* OtherDataReference) const { return this->DataReference == OtherDataReference; }

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	FItemDataBase* DataReference;
};