// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/InventoryTooltip.h"

// User Defined
#include "Objects/Items/ItemBase.h"
#include "UIs/Inventory/ItemSlotWidget.h"

// UE
#include "Components/TextBlock.h"


void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();
	Refresh();
	
}

void UInventoryTooltip::Refresh()
{
	if (!SourceSlot)
	{
		LOG_WARNING("SourceSlot is nullptr");
		return;
	}
	
	if (const TObjectPtr<UItemBase>& SourceItem = SourceSlot->Item)
	{
		Name->SetText(SourceItem->GetItemName());
		Description->SetText(SourceItem->GetStaticData()->Description);
		StackSize->SetText(FText::AsNumber(SourceSlot->Quantity));
		StackWeight->SetText(FText::AsNumber(SourceSlot->Quantity * SourceItem->GetWeight()));
		SellValue->SetText(FText::AsNumber(SourceItem->GetStaticData()->SellValue));

		switch (SourceItem->GetStaticData()->Quality)
		{
		case EItemQuality::Normal:
			Quality->SetText(FText::FromString(TEXT("일반")));
			break;
		case EItemQuality::Rare:
			Quality->SetText(FText::FromString(TEXT("희귀")));
			break;
		case EItemQuality::Unique:
			Quality->SetText(FText::FromString(TEXT("유일")));
			break;
		case EItemQuality::Legendary:
			Quality->SetText(FText::FromString(TEXT("전설")));
			break;
		case EItemQuality::Mythical:
			Quality->SetText(FText::FromString(TEXT("신화")));
			break;
		}

		switch (SourceItem->GetStaticData()->Type)
		{
		case EItemType::Weapon:
			break;
		case EItemType::Amor:
			break;
		case EItemType::Shield:
			break;
		case EItemType::Spell:
			break;
		case EItemType::Mundane:
			break;
		case EItemType::Consumable:
			break;
		case EItemType::Quest:
			break;
		}

		if (SourceItem->GetStaticData()->bIsStackable)
		{
			StackSize->SetText(FText::AsNumber(SourceSlot->Quantity));
		}
		else
		{
			StackSize->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
