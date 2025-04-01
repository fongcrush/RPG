// Fill out your copyright notice in the Description page of Project Settings.

// User Defined
#include "Frameworks/RPGHUD.h"
#include "RPG/RPG.h"
#include "UIs/Others/MainWidget.h"
#include "UIs/Inventory/InventoryMenu.h"
#include "UIs/Interaction/InteractionWidget.h"

void ARPGHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainWidgetClass)
	{		
		MainWidget = CreateWidget<UMainWidget>(GetWorld(), MainWidgetClass);
		MainWidget->AddToViewport();
		MainWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		LOG_CALLINFO_WARNING(" MainWidgetClass is nullptr!");
	}

	if (InventoryMenuClass)
	{
		InventoryMenu = CreateWidget<UInventoryMenu>(GetWorld(), InventoryMenuClass);
		InventoryMenu->AddToViewport();
		InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		LOG_CALLINFO_WARNING("InventoryMenuClass is nullptr!");
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);		
	}
	else
	{
		LOG_CALLINFO_WARNING("InteractionWidgetClass is nullptr!");
	}
}

void ARPGHUD::ToggleInventory()
{
	if (InventoryMenu->GetVisibility() == ESlateVisibility::Visible)
	{
		HideInventory();
		
		const FInputModeGameOnly InputModeGameOnly;
		GetOwningPlayerController()->SetInputMode(InputModeGameOnly); // 게임 입력 모드로 변경
		GetOwningPlayerController()->SetShowMouseCursor(false); // 마우스 커서 숨기기
	}
	else
	{
		ShowInventory();
		
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		GetOwningPlayerController()->SetInputMode(InputMode); // 게임과 UI 입력 모드로 변경
		GetOwningPlayerController()->SetShowMouseCursor(true); // 마우스 커서 보이기
	}
}

void ARPGHUD::ShowInventory()
{
	if (InventoryMenu)
	{
		InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	}
}

void ARPGHUD::HideInventory()
{
	if (InventoryMenu)
	{
		InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ARPGHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ARPGHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ARPGHUD::UpdateInteractionWidget(const FInteractableData& InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}
