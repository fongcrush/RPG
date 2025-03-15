// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_ActivateCollider.h"

void UAnimNotifyState_ActivateCollider::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	
	Colliders = MeshComp->GetOwner()->GetComponentsByTag(UPrimitiveComponent::StaticClass(), ColliderTag);
	for (UActorComponent* const& Collider : Colliders)
	{
		Cast<UPrimitiveComponent>(Collider)->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	}
}

void UAnimNotifyState_ActivateCollider::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{		
	for (UActorComponent* const& Collider : Colliders)
	{
		Cast<UPrimitiveComponent>(Collider)->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
}
