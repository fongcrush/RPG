// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class UInventoryTooltip;
class UDragItemVisual;
class UItemStackBase;
/**
 * 인벤토리의 각 아이템 항목(칸)을 나타내는 위젯 클래스
 */
UCLASS()
class RPG_API UInventoryItemSlot : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetItemReference(UItemStackBase* ItemIn) { ItemReference = ItemIn; }
	FORCEINLINE TObjectPtr<UItemStackBase> GetItemReference() const { return ItemReference; }

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot", meta=(BlueprintBaseOnly))
	TSubclassOf<UInventoryTooltip> TooltipClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot", meta=(BlueprintBaseOnly))
	TSubclassOf<UDragItemVisual> DragItemVisualClass;
	
	UPROPERTY(VisibleAnywhere, Category="Inventory Slot")
	TObjectPtr<UItemStackBase> ItemReference;
	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
