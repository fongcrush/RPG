// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/Interaction.h"
#include "RPGHUD.generated.h"

class UMainWidget;
class UInteractionWidget;
class UInventoryMenu;

/**
 * 
 */
UCLASS()
class RPG_API ARPGHUD : public AHUD
{
	GENERATED_BODY()

public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ToggleInventory();
	
	void UpdateInteractionWidget(const FInteractableData& InteractableData) const;
	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;

	FORCEINLINE UMainWidget* GetMainWidget() const { return MainWidget; }
	FORCEINLINE UInventoryMenu* GetInventoryMenu() const { return InventoryMenu; }
	FORCEINLINE UInteractionWidget* GetInteractionWidget() const { return InteractionWidget; }

protected:
	virtual void BeginPlay() override;
	
	void ShowInventory();
	void HideInventory();

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UMainWidget> MainWidgetClass;
	UPROPERTY()
	TObjectPtr<UMainWidget> MainWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UInventoryMenu> InventoryMenuClass;
	UPROPERTY()
	TObjectPtr<UInventoryMenu> InventoryMenu;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;
	UPROPERTY()
	TObjectPtr<UInteractionWidget> InteractionWidget;
	
};
