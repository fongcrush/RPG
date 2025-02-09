// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryTooltip.generated.h"

class UTextBlock;
class UInventoryItemSlot;
/**
 * 
 */
UCLASS()
class RPG_API UInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInventoryItemSlot> SlotBeingHovered;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Name;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Type;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Quality;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> UsageText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Description;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StackSize;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SellValue;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StackWeight;

	virtual void NativeConstruct() override;
};
