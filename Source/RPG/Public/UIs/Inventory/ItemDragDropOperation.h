// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UInventorySlotWidget;
class UItemStackBase;
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
	TObjectPtr<UItemStackBase> SourceItemStack;

	UPROPERTY()
	TObjectPtr<UInventorySlotWidget> SourceSlotWidget;
	
};
