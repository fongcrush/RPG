// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UInventorySlotWidget;
class UItemBase;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class RPG_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UInventorySlotWidget> SourceSlotWidget;

	UPROPERTY()
	int32 Quantity;
	
};
