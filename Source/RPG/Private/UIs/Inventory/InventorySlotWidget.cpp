// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/InventorySlotWidget.h"

// UE
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// User Defined
#include "Items/ItemBase.h"
#include "UIs/Inventory/DragItemVisual.h"
#include "UIs/Inventory/InventoryTooltip.h"
#include "UIs/Inventory/ItemDragDropOperation.h"


void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (TooltipClass)
	{
		Tooltip = CreateWidget<UInventoryTooltip>(this, TooltipClass);
		Tooltip->SourceSlot = this;
	}
	else
	{
		LOG_WARNING("TooltipClass is nullptr")
	}
	Reset();
}

void UInventorySlotWidget::Reset()
{
	Item = nullptr;;
	Quantity = 0;
	ItemBorder->Background.OutlineSettings.Color = FLinearColor::White;
	ItemIcon->SetColorAndOpacity(FLinearColor::Transparent);
	QuantityText->SetVisibility(ESlateVisibility::Collapsed);
	GetToolTip()->SetVisibility(ESlateVisibility::Collapsed);
	SetToolTip(nullptr);
}

void UInventorySlotWidget::Drop(const int32& QuantityDropped)
{
	if (Item)
	{
		LOG_CALLINFO("슬롯에 아이템이 없습니다.")
	}
	AActor* Owner = GetWorld()->GetFirstPlayerController()->GetPawn();
	UItemBase::GetValidItem(Item)->Drop(Owner, Quantity);
	Quantity = FMath::Max(0, Quantity - QuantityDropped);
	if (Quantity == 0)
	{
		Item = nullptr;
	}
	Refresh();
}

void UInventorySlotWidget::Refresh()
{
	if (Quantity <= 0 && !Item)
	{
		Reset();
		return;
	}
	
	switch (Item->GetStaticData()->Quality)
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
	default: ;
	}
	if (UTexture2D* Icon = Item->GetStaticData()->AssetData.Icon)
	{
		ItemIcon->SetBrushFromTexture(Icon);
	}
	if (Item->IsStackable())
	{
		QuantityText->SetText(FText::AsNumber(Quantity));
		QuantityText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		QuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Tooltip)
	{
		Tooltip->Refresh();
		SetToolTip(Tooltip);
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

	if (DragItemVisualClass && Item)
	{
		UDragItemVisual* DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(Item->GetStaticData()->AssetData.Icon);
		DragVisual->ItemQuantity->SetText(FText::AsNumber(Quantity));

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
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
		const ItemPtr& SourceItem = SourceSlotWidget->Item;
		const int32& SourceQuantity = DragOperation->Quantity;
		// 기존 아이템이 있는 경우
		if (Item)
		{
			// 같은 슬롯일 경우 패스
			if (SourceSlotWidget == this)
			{
				return true;
			}
			// 같은 아이템일 경우
			if (Item->GetStaticData() == SourceItem->GetStaticData())
			{
				// 쌓을 수 없으면 패스
				if (!Item->IsStackable())
				{
					return true;
				}
				// 스택이 꽉 차있다면 자리만 바꾼다
				if (Quantity >= Item->GetMaxSize())
				{
					int32 tempQuantity = Quantity;
					Quantity = SourceQuantity;
					SourceSlotWidget->Quantity = tempQuantity;

					Refresh();
					SourceSlotWidget->Refresh();
					return true;
				}

				// 스택을 채워넣고 남은 건 원래 슬롯에 남긴다 
				const int32 SumQuantity = Quantity + SourceQuantity;
				const int32 NewQuantity = FMath::Min(SumQuantity, Item->GetMaxSize());
				const int32 LeftQuantity = SumQuantity - NewQuantity;

				Quantity = NewQuantity;
				SourceSlotWidget->Quantity = LeftQuantity;

				Refresh();
				SourceSlotWidget->Refresh();
				return true;
			}
			// 다른 아이템일 경우 자리만 바꿈
			ItemPtr TempItem = Item;
			Item = SourceItem;
			SourceSlotWidget->Item = TempItem;

			int32 TempQuantity = Quantity;
			Quantity = SourceQuantity;
			SourceSlotWidget->Quantity = TempQuantity;

			Refresh();
			SourceSlotWidget->Refresh();
			return true;
		}
		// 슬롯에 아이템이 없으면 추가
		Item = SourceItem;		
		Refresh();
		SourceSlotWidget->Reset();
		return true;
	}
	return true;
}
