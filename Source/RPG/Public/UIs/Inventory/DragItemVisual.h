// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"


class UTextBlock;
class UImage;
class UItemBase;

/**
 * 
 */
UCLASS()
class RPG_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
	
	UPROPERTY(VisibleAnywhere, Category="Inventory Slot")
	TObjectPtr<UItemBase> Item;
};
