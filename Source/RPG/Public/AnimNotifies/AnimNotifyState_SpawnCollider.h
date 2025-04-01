// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SpawnCollider.generated.h"

/**
 * 
 */
UCLASS(editinlinenew, Blueprintable, Const, hidecategories = Object, collapsecategories, meta = (ShowWorldContextPin, DisplayName = "Spawn Collider"))
class RPG_API UAnimNotifyState_SpawnCollider : public UAnimNotifyState
{
	GENERATED_BODY()
	/**
	 * ====== 주요 매개 변수 ======
	 * UAnimSequenceBase : Montage, Sequence, Composite 등, 파생 타입을 파악한 후 처리한다. 애니메이션의 현재 위치, 기타 등등
	 * USkeletalMeshComponent : GetWorld(), GetAnimInstance()
	 * FAnimNotifyEvent : 노티파이의 실질적인 인스턴스. 트리거 시작 시간, 종료 시간, NotifyName 등, 대부분의 정보 참조 가능.
	 * FAnimNotifyEventReference : UAnimNotifyLibrary 의 입력용 변수로 활용 됨.
	 */

	/**
	 * ====== 애니메이션 씬의 시각화 ======
	 * 애니메이션 씬같은 Persona Asset의 뷰포트는 임시 World를 사용한다. 임시 월드는 에셋이 에디터에서 열릴때마다 새로 생성된다.
	 * NotifyTick()의 매개변수 FAnimNotifyEvent& NotifyEvent 는 애니매이션 씬에서 참조할 경우 초기화 되지 않은 쓰레기 값이다.
	 * 대안으로 OnAnimNotifyCreatedInEditor()의 매개변수 FAnimNotifyEvent& 를 참조하거나, DrawInEditor() 에서 로직을 처리한다.
	 * DrawInEditor() 는 읽기 전용(const) 이므로 멤버에 mutable 을 붙여서 사용한다.
	 * 시퀸서
	 */

	/**
	 * ====== 그 밖에 =======
	 * FAnimNotifyDetails, FAnimationRecorder, Persona 툴 관련 구현, 확인할 가치가 있을 듯
	 */
	
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UAnimNotifyState_SpawnCollider();

protected:
#if WITH_EDITOR
	/** 소켓 이름을 가져오기 위한 함수. GetOptions 프로퍼티 지정자에 사용 함. */
	UFUNCTION()
	TArray<FName> GetSocketNames() const;
	/** 에셋 검사. ActorClass가 None일 경우 경고 메시지. */
	virtual void ValidateAssociatedAssets() override;
	/** 애니메이션 시퀸스 에디터의 Notify 시각화에 사용. */
	virtual void DrawInEditor(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* MeshComp, const UAnimSequenceBase* Animation, const FAnimNotifyEvent& NotifyEvent) const override;
#endif

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)	override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,	const FAnimNotifyEventReference& EventReference) override;

protected:
	/** 생성한 엑터 저장/조회를 위한 키 조회 함수.
	 * 노티파이는 애니메이션/몽타주 단위로 생성되고 공유되므로, 런타임에 수정되는 맴버를 가져서는 안 된다.
	 * 따라서 각 캐릭터에 키맵으로 저장하여 사용한다. */
	uint32 GetNotifyKey(UAnimSequenceBase* Animation) { return HashCombine(GetTypeHash(this), GetTypeHash(Animation)); }
	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	TSubclassOf<AActor> ActorClass;
	
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	bool bAttached;
	
	UPROPERTY(EditAnywhere, Category = "AnimNotify", meta = (EditCondition = "bAttached", EditConditionHides, GetOptions = "GetSocketNames"))
	FName SocketName;
	
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	FVector Location;

#if WITH_EDITORONLY_DATA
	// 엑터 위치 표시용 디버그 Sphere 지속 시간
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	float DrawDuration = 0.05f;
	
	// 에디터 전용 참조
	mutable TWeakObjectPtr<AActor> SpawnedEditorOnly;
	// Notify가 시퀸서에서 지워지면, 생성한 엑터를 제거하기 위해 사용
	mutable FTimerHandle RemoveCheckHandle;
#endif
};
