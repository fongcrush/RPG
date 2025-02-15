// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

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
	UCanvas* MainCanvas;
	
	UPROPERTY()
	TObjectPtr<ARPGCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UInventoryWidget> MainInventoryWidget;
	
	UPROPERTY()
	TArray<TObjectPtr<UInventoryWidget>> SubInventoryWidgets;
	
	UPROPERTY()
	TArray<TObjectPtr<UInventoryWidget>> TempInventoryWidgets;

protected:
	
public:
	void InitializeMainInventory();
	void AddSubInventory(UInventoryWidget* SubInventoryWidget);
	

protected:
	virtual void NativeOnInitialized() override;
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
