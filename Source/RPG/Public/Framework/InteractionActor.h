// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_Interaction.h"
#include "InteractionActor.generated.h"

class ARPGCharacter;

UCLASS()
class RPG_API AInteractionActor : public AActor, public IInterface_Interaction
{
	GENERATED_BODY()

public:
	AInteractionActor();

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ARPGCharacter* const& InteractionCharacter) override;
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Interaction Actor")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	virtual void BeginPlay() override;
};
