// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/RPGHUD.h"

#include "UIs/InventoryMenu.h"
#include "UIs/Interaction/InteractionWidget.h"

void ARPGHUD::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryMenuClass)
	{
		InventoryMenu = CreateWidget<UInventoryMenu>(GetWorld(), InventoryMenuClass);
		InventoryMenu->AddToViewport();
		InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);		
	}
}

void ARPGHUD::ToggleInventory()
{
	if (bIsMenuVisible)
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
		bIsMenuVisible = true;
		InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	}
}

void ARPGHUD::HideInventory()
{
	if (InventoryMenu)
	{
		bIsMenuVisible = false;
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
