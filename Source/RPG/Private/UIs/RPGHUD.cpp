// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/RPGHUD.h"

#include "UIs/MainMenu.h"
#include "UIs/Interaction/InteractionWidget.h"

void ARPGHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuWidgetClass);
		MainMenuWidget->AddToViewport();
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);		
	}
}

void ARPGHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ARPGHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ARPGHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();
		
		const FInputModeGameOnly InputModeGameOnly;
		GetOwningPlayerController()->SetInputMode(InputModeGameOnly); // 게임 입력 모드로 변경
		GetOwningPlayerController()->SetShowMouseCursor(false); // 마우스 커서 숨기기
	}
	else
	{
		DisplayMenu();
		
		const FInputModeGameAndUI InputModeGameAndUI;
		GetOwningPlayerController()->SetInputMode(InputModeGameAndUI); // 게임과 UI 입력 모드로 변경
		GetOwningPlayerController()->SetShowMouseCursor(true); // 마우스 커서 보이기
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
