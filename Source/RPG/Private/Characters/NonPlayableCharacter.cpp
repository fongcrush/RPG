// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NonPlayableCharacter.h"

ANonPlayableCharacter::ANonPlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANonPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void ANonPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

