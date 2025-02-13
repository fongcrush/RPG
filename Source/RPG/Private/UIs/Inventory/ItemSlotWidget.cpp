// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/ItemSlotWidget.h"

// UE
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// User Defined
#include "Items/ItemStackBase.h"
#include "UIs/Inventory/DragItemVisual.h"
#include "UIs/Inventory/InventoryTooltip.h"
#include "UIs/Inventory/ItemDragDropOperation.h"


void UItemSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (TooltipClass)
	{
		UInventoryTooltip* Tooltip = CreateWidget<UInventoryTooltip>(this, TooltipClass);
		Tooltip->SlotBeingHovered = this;
		SetToolTip(Tooltip);
	}
	Reset();
}

void UItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemSlotWidget::Reset()
{
	ItemReference = nullptr;;
	ItemBorder->Background.OutlineSettings.Color = FLinearColor::White;
	ItemIcon->SetColorAndOpacity(FLinearColor::Transparent);
	QuantityText->SetText(FText::GetEmpty());
}

void UItemSlotWidget::Refresh()
{
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
			QuantityText->SetText(FText::FText::AsNumber(ItemReference->Quantity));
		}
		else
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply;
}

void UItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragItemVisualClass && ItemReference)
	{
		UDragItemVisual* DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->GetDataReference()->AssetData.Icon);
		DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::CenterCenter;

		OutOperation = DragItemOperation;
		
	}
}

bool UItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
