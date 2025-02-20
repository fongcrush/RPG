// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/InteractionActor.h"
#include "Framework/RPGCharacter.h"


// Sets default values
AInteractionActor::AInteractionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AInteractionActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractionActor::BeginFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInteractionActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AInteractionActor::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("AInteractionActor::BeginInteract"));
}

void AInteractionActor::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("AInteractionActor::EndInteract"));
}

void AInteractionActor::Interact(ARPGCharacter* const& InteractionCharacter)
{
	IInteraction::Interact(InteractionCharacter);
}

