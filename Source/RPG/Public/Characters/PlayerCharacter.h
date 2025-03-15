// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Framework/RPGCharacterBase.h"
#include "PlayerCharacter.generated.h"

class UInteractorComponent;
class UItemBase;
class UInventoryComponent;
class IInteraction;
class USpringArmComponent;
class UCameraComponent;
class ARPGHUD;
class UInputMappingContext;
class UInputAction;

UCLASS(PrioritizeCategories=("Character", "Inventory", "Interact", "Input"))
class RPG_API APlayerCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()
	
public:
#pragma region public_functions
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	UFUNCTION()
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera;}
	
	UFUNCTION()
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	
protected:
#pragma endregion
#pragma region protected_functions
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = "Character", DisplayName = "Walk")
	void K2_Walk(const FVector2D& Value);
	
	void Walk(const FInputActionInstance& Instance);
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	void Look(const FInputActionInstance& Instance);
	
#pragma endregion
#pragma region protected_variables
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ARPGHUD> HUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float TurnRateGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UInteractorComponent> InteractorComponent;

	// Input Mapping
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
#pragma endregion
	
};
