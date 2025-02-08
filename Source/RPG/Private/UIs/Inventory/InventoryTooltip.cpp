// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/InventoryTooltip.h"

// UE
#include "Components/TextBlock.h"

// User Defined
#include "Items/ItemBase.h"
#include "UIs/Inventory/InventoryItemSlot.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotBeingHovered)
	{
		if (const TObjectPtr<UItemBase>& ItemBeingHovered = SlotBeingHovered->GetItemReference())
		{
			Name->SetText(ItemBeingHovered->GetItemName());
			Description->SetText(ItemBeingHovered->GetDataReference()->Description);
			StackSizeText->SetText(FText::AsNumber(ItemBeingHovered->Quantity));
			StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));
			SellValue->SetText(FText::AsNumber(ItemBeingHovered->GetDataReference()->SellValue));

			switch (ItemBeingHovered->GetDataReference()->Quality)
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
		}
	}
}
