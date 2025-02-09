// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDataStructs.h"
#include "ItemBase.generated.h"

class UInventoryComponent;

UCLASS()
class RPG_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY()
	UInventoryComponent* OwingInventory;

	UPROPERTY(EditAnywhere, Category = "Item")
	FDataTableRowHandle ItemDataHandle;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;
	
	bool bIsCopy, bIsPickup;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UItemBase();
	
	virtual void PostInitProperties() override;
	
	UItemBase* CreateItemCopy() const;
	
	FORCEINLINE void ResetItemFlags() { bIsCopy = false; bIsPickup = false; }

	FORCEINLINE FItemDataBase* GetDataReference() const { return DataReference; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE FText GetItemName() const { return DataReference->Name; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * DataReference->Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return DataReference->Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullStack() const { return Quantity == DataReference->MaxStackSize; }

	FORCEINLINE bool IsStackable() const { return DataReference->bIsStackable; }
	
	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(class ARPGCharacter* Character);

	bool operator==(const FName& OtherRowName) const { return this->ItemDataHandle.RowName == OtherRowName; }

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	FItemDataBase* DataReference;
};