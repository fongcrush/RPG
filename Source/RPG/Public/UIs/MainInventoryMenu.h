// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainInventoryMenu.generated.h"

class UScrollBox;
class UCanvasPanel;
class UInventoryWidget;
class ARPGCharacter;
/**
 * 
 */
UCLASS()
class RPG_API UMainInventoryMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<TObjectPtr<UInventoryWidget>> InventoryWidgets;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> InventoryPanel;
	

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
