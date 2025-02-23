// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/ItemSlotWidget.h"

// UE
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// User Defined
#include "Items/ItemBase.h"
#include "UIs/Inventory/DragItemVisual.h"
#include "UIs/Inventory/InventoryTooltip.h"
#include "UIs/Inventory/ItemDragDropOperation.h"


void UItemSlotWidget::NativeOnInitialized()
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

void UItemSlotWidget::Drop(const int32& QuantityDropped)
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

void UItemSlotWidget::Reset()
{
	Item = nullptr;;
	Quantity = 0;
	ItemBorder->Background.OutlineSettings.Color = FLinearColor::White;
	ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	QuantityText->SetVisibility(ESlateVisibility::Collapsed);
	SetToolTip(nullptr);
}

void UItemSlotWidget::Refresh()
{
	if (!Item || Quantity <= 0)
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
		ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if (Item->IsStackable())
	{
		QuantityText->SetText(FText::AsNumber(Quantity));
		QuantityText->SetVisibility(ESlateVisibility::HitTestInvisible);
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

FReply UItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::RightMouseButton);
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragItemVisualClass && Item)
	{
		UDragItemVisual* DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(Item->GetStaticData()->AssetData.Icon);
		DragVisual->ItemQuantity->SetText(FText::AsNumber(Quantity));

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->Pivot = EDragPivot::CenterCenter;
		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Quantity = Quantity;
		DragItemOperation->SourceSlot = this;
		
		OutOperation = DragItemOperation;
	}
}

bool UItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (const UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(InOperation))
	{
		/**
		 * 1. 이 슬롯에 아이템이 없는 경우
		 * 2. 아이템이 있는 경우
		 * 2.1. 아이템이 다른 경우
		 * 2.2. 아이템이 같은 경우
		 * 2.2.1. 현재 슬롯에 수량이 가득 차 있는 경우 
		 */

		const ItemSlotPtr& SourceSlot = DragOperation->SourceSlot;
		const ItemPtr& SourceItem = SourceSlot->Item;
		const int32& SourceQuantity = DragOperation->Quantity;
		// 현재 슬롯일 경우 넘어감
		if (SourceSlot == this)
		{
			return true;
		}
		// 현재 슬롯에 아이템이 없다면 아이템 옮김
		if (!Item)
		{
			Item = SourceItem;
			Quantity = SourceQuantity;
			
			Refresh();
			SourceSlot->Refresh();
			return true;
		}
		// 아이템이 다르면 서로 바꿈
		if (Item != SourceItem)
		{
			const ItemPtr TempItem = Item;
			Item = SourceItem;
			SourceSlot->Item = TempItem;

			const int32 TempQuantity = Quantity;
			Quantity = SourceQuantity;
			SourceSlot->Quantity = TempQuantity;

			Refresh();
			SourceSlot->Refresh();
			return true;
		}
		
		// 같은 아이템일 경우
		// 쌓을 수 없으면(최대 수량 1이면) 패스
		if (!Item->IsStackable())
		{
			return true;
		}
		// 현재 슬롯에 수량이 가득 차있다면 자리만 바꾼다
		if (IsFull())
		{
			const int32 TempQuantity = Quantity;
			Quantity = SourceQuantity;
			SourceSlot->Quantity = TempQuantity;

			Refresh();
			SourceSlot->Refresh();
			return true;
		}

		// 스택을 채워넣고 남은 건 원래 슬롯에 남긴다 
		const int32 SumQuantity = Quantity + SourceQuantity;
		const int32 NewQuantity = FMath::Min(SumQuantity, Item->GetMaxSize());
		const int32 LeftQuantity = SumQuantity - NewQuantity;

		Quantity = NewQuantity;
		SourceSlot->Quantity = LeftQuantity;

		Refresh();
		SourceSlot->Refresh();
		return true;
	}
	return true;
}

bool UItemSlotWidget::IsFull() const
{
	return Item->IsStackable() ? Quantity >= Item->GetMaxSize() : Quantity > 0;
}

int32 UItemSlotWidget::GetEmptySize() const
{
	return Item->GetMaxSize() - Quantity; 
}