// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_SpawnActor.h"
#include "AnimNotify_SpawnCollider.generated.h"

/**
 * 단 한 틱만 살아있는 콜라이더를 생성하는 노티파이. 순간적인 공격에 사용하기 적합합니다.
 */
UCLASS(editinlinenew, Blueprintable, Const, hidecategories = Object, collapsecategories, meta = (ShowWorldContextPin, DisplayName = "Spawn Collider"))
class RPG_API UAnimNotify_SpawnCollider : public UAnimNotify_SpawnActor
{
	GENERATED_BODY()

#if WITH_EDITOR
	/** 애니메이션 시퀸스 에디터의 Notify 시각화에 사용. */
	virtual void DrawInEditor(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* MeshComp, const UAnimSequenceBase* Animation, const FAnimNotifyEvent& NotifyEvent) const override;
#endif
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
