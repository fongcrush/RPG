// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ActivateCollider.generated.h"

/**
 * 
 */
UCLASS(editinlinenew, Blueprintable, Const, hidecategories = Object, collapsecategories, MinimalAPI, meta = (ShowWorldContextPin, DisplayName = "Activate Collider"))
class UAnimNotifyState_ActivateCollider : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	FName ColliderTag;
};
