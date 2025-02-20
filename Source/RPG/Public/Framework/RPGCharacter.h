// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "RPGCharacter.generated.h"

class UItemBase;
class UInventoryComponent;
class IInteraction;
class USpringArmComponent;
class UCameraComponent;
class ARPGHUD;
class UInputMappingContext;
class UInputAction;

UCLASS(meta=(PrioritizeCategories="Character Input"))
class RPG_API ARPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
#pragma region public_functions
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	ARPGCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	UFUNCTION()
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera;}
	
	UFUNCTION()
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	
	UFUNCTION()
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(InteractionTimerHandle);}

	void UpdateInteractionWidget() const;
#pragma endregion
	
protected:
#pragma region protected_variables
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY()
	TObjectPtr<ARPGHUD> HUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float TurnRateGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere, Category = "Character")
	TScriptInterface<IInteraction> TargetInteractable;
	
	UPROPERTY(VisibleAnywhere, Category = "Character")
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
	
	UPROPERTY()
	float InteractionCheckFrequency;

	UPROPERTY()
	float InteractionCheckDistance;

	FTimerHandle InteractionTimerHandle;

	// Input Mapping
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> UIToggleAction;
#pragma endregion
#pragma region protected_functions
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓	
	virtual void BeginPlay() override;
	
	void PerformInteractionCheck();
	void NoInteractableFound();
	void FoundInteractable(AActor* Target);
	void BeginInteract();
	void EndInteract();
	void Interact();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void ToggleMenu();
#pragma endregion
};
