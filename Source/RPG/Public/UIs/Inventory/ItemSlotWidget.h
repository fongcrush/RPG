// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class UInventoryWidget;
class UTextBlock;
class UImage;
class UBorder;
class UInventoryTooltip;
class UDragItemVisual;
class UItemBase;
/**
 * 인벤토리의 각 아이템 항목(칸)을 나타내는 위젯 클래스
 */
UCLASS()
class RPG_API UItemSlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void Drop(const int32& QuantityDropped);
	virtual void Refresh();
	virtual void Reset();
	
	virtual void SetParent(UInventoryWidget* Parent) { InventoryWidget = Parent; }
	static bool Compare(const UItemSlotWidget& ItemSlotA, const UItemSlotWidget& ItemSlotB) { return ItemSlotA.Index < ItemSlotB.Index; }
	FORCEINLINE bool IsFull() const;
	FORCEINLINE int32 GetEmptySize() const;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(VisibleInstanceOnly, Category="Slot")
	int32 Index;
	
	UPROPERTY(VisibleInstanceOnly, Category="Slot")
	TObjectPtr<UItemBase> Item;
	
	UPROPERTY(VisibleInstanceOnly, Category="Slot")
	int32 Quantity;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Slot", meta=(BlueprintBaseOnly))
	TSubclassOf<UInventoryTooltip> TooltipClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Slot", meta=(BlueprintBaseOnly))
	TSubclassOf<UDragItemVisual> DragItemVisualClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityText;

	UPROPERTY()
	UInventoryTooltip* Tooltip;

	TObjectPtr<UInventoryWidget> InventoryWidget; 
};

using ItemSlotPtr = TObjectPtr<UItemSlotWidget>;
