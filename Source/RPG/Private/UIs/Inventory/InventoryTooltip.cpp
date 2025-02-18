// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/InventoryTooltip.h"

// UE
#include "Components/TextBlock.h"

// User Defined
#include "Items/ItemStackBase.h"
#include "UIs/Inventory/InventorySlotWidget.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotBeingHovered)
	{
		if (const TObjectPtr<UItemStackBase>& ItemBeingHovered = SlotBeingHovered->GetItemStack())
		{
			Name->SetText(ItemBeingHovered->GetItemName());
			Description->SetText(ItemBeingHovered->GetStaticData()->Description);
			StackSize->SetText(FText::AsNumber(ItemBeingHovered->Quantity));
			StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetStackWeight()));
			SellValue->SetText(FText::AsNumber(ItemBeingHovered->GetStaticData()->SellValue));

			switch (ItemBeingHovered->GetStaticData()->Quality)
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

			switch (ItemBeingHovered->GetStaticData()->Type)
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

			if (ItemBeingHovered->GetStaticData()->bIsStackable)
			{
				StackSize->SetText(FText::AsNumber(ItemBeingHovered->Quantity));
			}
			else
			{
				StackSize->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}
