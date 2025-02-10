// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/InventoryItemSlot.h"

// UE
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// User Defined
#include "Components/TileView.h"
#include "Items/ItemBase.h"
#include "UIs/Inventory/DragItemVisual.h"
#include "UIs/Inventory/InventoryTooltip.h"


void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (TooltipClass)
	{
		UInventoryTooltip* Tooltip = CreateWidget<UInventoryTooltip>(this, TooltipClass);
		Tooltip->SlotBeingHovered = this;
		SetToolTip(Tooltip);
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemReference)
	{
		switch (ItemReference->GetDataReference()->Quality)
		{
		case EItemQuality::Normal:
			ItemBorder->Background.OutlineSettings.Color = FLinearColor::White;
			break;
		case EItemQuality::Rare:
			ItemBorder->Background.OutlineSettings.Color = FLinearColor::Green;
			break;
		case EItemQuality::Unique:
			ItemBorder->Background.OutlineSettings.Color = FLinearColor::Blue;
			break;
		case EItemQuality::Legendary:
			ItemBorder->Background.OutlineSettings.Color = FLinearColor::Red;
			break;
		case EItemQuality::Mythical:
			ItemBorder->Background.OutlineSettings.Color = FLinearColor::Yellow;
			break;
		default:;
		}
		ItemIcon->SetBrushFromTexture(ItemReference->GetDataReference()->AssetData.Icon);

		if (ItemReference->GetDataReference()->bIsStackable)
		{
			ItemQuantity->SetText(FText::FText::AsNumber(ItemReference->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
