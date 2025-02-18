// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UCanvasPanel;
class UInventoryWidget;
class ARPGCharacter;
/**
 * 
 */
UCLASS()
class RPG_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> MainCanvas;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInventoryWidget> MainInventoryWidget;
	
	UPROPERTY()
	TArray<TObjectPtr<UInventoryWidget>> SubInventoryWidgets;
	
	UPROPERTY()
	TObjectPtr<ARPGCharacter> PlayerCharacter;


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
