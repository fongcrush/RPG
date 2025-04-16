// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Others/DeferredActor.h"

ADeferredActor::ADeferredActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADeferredActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADeferredActor::DeferredBegin()
{
	
}

// Called every frame
void ADeferredActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

