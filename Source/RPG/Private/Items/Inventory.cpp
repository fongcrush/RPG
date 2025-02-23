// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory.h"

// UE
#include "Blueprint/UserWidget.h"

// User Defined
#include "Components/ScrollBox.h"
#include "RPG/RPG.h"
#include "UIs/InventoryMenu.h"
#include "UIs/RPGHUD.h"
#include "UIs/Inventory/InventoryWidget.h"


UInventory::UInventory()
{
}

void UInventory::Use(AActor* Owner)
{
	Super::Use(Owner);
	if (bIsOwned || !Posses())
	{
		return;
	}
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
}

bool UInventory::Posses()
{
	if (!InventoryWidget)
	{
		InventoryStaticData = StaticDataHandle.GetRow<FInventoryStaticData>(GetName());
		if (!InventoryStaticData)
		{
			LOG_CALLINFO("인벤토리 정적 데이터가 할당되지 않았습니다.");
			return false;
		}
		if (!InitializeWidget())
		{
			LOG_CALLINFO_WARNING("인번토리 위젯 초기화에 실패했습니다. 정적 데이터의 위젯 클래스가 할당되지 않았습니다.");
			return false;
		}
	}
	if (!GetInventoryMenu()->HasChild(InventoryWidget))
	{
		GetInventoryMenu()->AddChild(InventoryWidget);
	}
	InventoryMenu->InventoryWidgets.AddUnique(InventoryWidget);
	bIsOwned = true;
	return true;
}

bool UInventory::InitializeWidget()
{
	// 위젯 바인딩 확인
	if (!InventoryStaticData->InventoryWidgetClass)
	{
		return false;
	}
	// 위젯 생성, 인벤토리 메뉴에 등록
	const APlayerController* const& PlayerController = GetWorld()->GetFirstPlayerController<APlayerController>();
	InventoryMenu = Cast<ARPGHUD>(PlayerController->GetHUD())->InventoryMenu;
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryStaticData->InventoryWidgetClass);
	if (InventoryWidget)
	{
		InventoryMenu->InventoryPanel->AddChild(InventoryWidget);
		InventoryMenu->InventoryWidgets.Add(InventoryWidget);
	}
	return true;
}

void UInventory::Drop(AActor* const& Owner, const int32& QuantityToDrop)
{
	Super::Drop(Owner, QuantityToDrop);
	// 위젯 해제
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	InventoryMenu->InventoryPanel->RemoveChild(InventoryWidget);
	InventoryMenu->InventoryWidgets.Remove(InventoryWidget);
	bIsOwned = false;
}

float UInventory::GetWeight() const
{
	return InventoryWidget->GetWeight();
}