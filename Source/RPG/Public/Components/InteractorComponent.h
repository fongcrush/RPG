// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"


class IInteraction;
class ARPGHUD;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractorComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	void BeginInteract();
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	void EndInteract();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PerformInteractionCheck();
	void NoInteractableFound();
	void FoundInteractable(AActor* Target);
	
	void Interact();
	
	FORCEINLINE bool IsInteracting() const { return GetOwner()->GetWorldTimerManager().IsTimerActive(InteractionTimerHandle);}

protected:
	UPROPERTY(VisibleAnywhere, Category="Interact")
	TScriptInterface<IInteraction> TargetInteractable;

	UPROPERTY(EditAnywhere, Category="Interact")
	float InteractionCheckDistance;
	
	UPROPERTY()
	float LastInteractionCheckTime;
	
	UPROPERTY()
	float InteractionCheckFrequency;
	
	FTimerHandle InteractionTimerHandle;
	
	UPROPERTY()
	TObjectPtr<ARPGHUD> HUD;

	UPROPERTY()
	TObjectPtr<APawn> ParentPawn;
};
