// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "InteractionActor.generated.h"

class ARPGCharacter;

UCLASS()
class RPG_API AInteractionActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AInteractionActor();

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(TObjectPtr<ARPGCharacter> InteractionCharacter) override;
	virtual FInteractableData* GetInteractableData() override { return &InteractableData; }
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Interaction Actor")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Interaction Actor")
	FInteractableData InteractableData;
	
	virtual void BeginPlay() override;
};
