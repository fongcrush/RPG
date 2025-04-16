// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SpawnActor.generated.h"

/**
 * 
 */
UCLASS(editinlinenew, Blueprintable, Const, hidecategories = Object, collapsecategories, meta = (ShowWorldContextPin, DisplayName = "Spawn Actor"))
class RPG_API UAnimNotify_SpawnActor : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UAnimNotify_SpawnActor();

protected:
#if WITH_EDITOR
	/** 소켓 이름을 가져오기 위한 함수. GetOptions 프로퍼티 지정자에 사용 함.*/
    UFUNCTION()
	TArray<FName> GetSocketNames() const;
	/** 에셋 검사. ActorClass가 None일 경우 경고 메시지.*/
	virtual void ValidateAssociatedAssets() override;
	// bAttached 설정이 변경되면 프리뷰 엑터 초기화
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	/** 애니메이션 시퀸스 에디터의 Notify 시각화에 사용. */
	virtual void DrawInEditor(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* MeshComp, const UAnimSequenceBase* Animation, const FAnimNotifyEvent& NotifyEvent) const override;
#endif
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	TSubclassOf<AActor> ActorClass;
	
	UPROPERTY(EditAnywhere, Category = "AnimNotify", meta = (InlineEditConditionToggle))
	bool bAttached;
	
	UPROPERTY(EditAnywhere, Category = "AnimNotify", meta = (EditCondition = "bAttached", GetOptions = "GetSocketNames"))
	FName SocketName;
	
	UPROPERTY(EditAnywhere, Category = "AnimNotify", meta = (MakeEditWidget = true))
	FVector Location;

#if WITH_EDITORONLY_DATA	
	// 에디터 전용 참조
	mutable TWeakObjectPtr<AActor> SpawnedEditorOnly;
	// Notify가 시퀸서에서 지워지면, 생성한 엑터를 제거하기 위한 용도
	mutable FTimerHandle RemoveCheckHandle;
#endif
};
