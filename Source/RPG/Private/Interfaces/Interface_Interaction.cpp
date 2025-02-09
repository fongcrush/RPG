// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/Interface_Interaction.h"
#include "Framework/RPGCharacter.h"


// Add default functionality here for any IInteractionInterface functions that are not pure virtual.
void IInterface_Interaction::BeginFocus()
{
}

void IInterface_Interaction::EndFocus()
{
}

void IInterface_Interaction::BeginInteract()
{
}

void IInterface_Interaction::EndInteract()
{
}

void IInterface_Interaction::Interact(TObjectPtr<ARPGCharacter> InteractionCharacter)
{
}