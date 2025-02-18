// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/InventorySlotWidget.h"

// UE
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// User Defined
#include "Items/ItemStackBase.h"
#include "UIs/Inventory/DragItemVisual.h"
#include "UIs/Inventory/InventoryTooltip.h"
#include "UIs/Inventory/ItemDragDropOperation.h"


void UInventorySlotWidget::NativeOnInitialized()
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

void UInventorySlotWidget::Reset()
{
	ItemStack = nullptr;;
	ItemBorder->Background.OutlineSettings.Color = FLinearColor::White;
	ItemIcon->SetColorAndOpacity(FLinearColor::Transparent);
	QuantityText->SetText(FText::GetEmpty());
}

void UInventorySlotWidget::Refresh()
{
	if (ItemStack->Quantity > 0 && ItemStack)
	{		
		switch (ItemStack->GetStaticData()->Quality)
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
		if (UTexture2D* Icon = ItemStack->GetStaticData()->AssetData.Icon)
		{
			ItemIcon->SetBrushFromTexture(Icon);
		}
		if (ItemStack->GetStaticData()->bIsStackable)
		{
			QuantityText->SetText(FText::AsNumber(ItemStack->Quantity));
		}
		else
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		Reset();
	}
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	const FKey Key = InMouseEvent.GetEffectingButton();
	
	if (Key == EKeys::LeftMouseButton)
	{
		Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	else if (Key == EKeys::LeftMouseButton)
	{		
		Reply.Handled().DetectDrag(TakeWidget(), EKeys::RightMouseButton);
	}
	return Reply;
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragItemVisualClass && ItemStack)
	{
		UDragItemVisual* DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemStack->GetStaticData()->AssetData.Icon);
		DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemStack->Quantity));

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItemStack = ItemStack;
		DragItemOperation->SourceSlotWidget = this;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::CenterCenter;

		OutOperation = DragItemOperation;
	}
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (const UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(InOperation))
	{
		/**
		 * 1. 아이템이 있는 경우
		 * 1.1. 아이템이 같은 경우
		 * 1.2. 아이템이 다른 경우
		 * 2. 아이템이 없는 경우
		 */
		
		const SlotWidgetPtr& SourceSlotWidget = DragOperation->SourceSlotWidget;
		const ItemStackPtr& SourceItemStack = DragOperation->SourceItemStack;
		// 기존 아이템이 있는 경우
		if (ItemStack)
		{
			// 같은 슬롯일 경우 패스
			if (SourceSlotWidget == this)
			{
				return true;
			}
			// 같은 아이템일 경우
			if (ItemStack->GetStaticData() == SourceItemStack->GetStaticData())
			{
				// 쌓을 수 없으면 패스
				if (!ItemStack->IsStackable())
				{
					return true;
				}
				// 스택이 꽉 차있다면 자리만 바꾼다
				if (ItemStack->IsFullStack())
				{
					ItemStack->SetQuantity(SourceItemStack->Quantity);					
					SourceItemStack->SetQuantity(ItemStack->Quantity);
					
					Refresh();
					SourceSlotWidget->Refresh();
					return true;
				}

				// 스택을 채워넣고 남은 건 원래 슬롯에 남긴다 
				const int32 SumQuantity = ItemStack->Quantity + SourceItemStack->Quantity;
				const int32 NewQuantity = FMath::Min(SumQuantity, ItemStack->GetMaxSize());
				const int32 LeftQuantity = SumQuantity - NewQuantity;
					
				ItemStack->SetQuantity(NewQuantity);					
				SourceItemStack->SetQuantity(LeftQuantity);
					
				Refresh();
				SourceSlotWidget->Refresh();
				return true;
			}
			
			ItemStackPtr TempItemStack = ItemStack;
			ItemStack = SourceItemStack;
			SourceSlotWidget->ItemStack = TempItemStack;
				
			Refresh();
			SourceSlotWidget->Refresh();
			return true;
		}
		
		ItemStack = SourceItemStack;
		Refresh();
		SourceSlotWidget->Reset();
		return true;
	}
	return true;
}
