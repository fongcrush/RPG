// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotifyState_SpawnCollider.h"

#include "RPG/RPG.h"
#include "Characters/RPGCharacterBase.h"

#include "Animation/AnimSingleNodeInstance.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Misc/UObjectToken.h"

UAnimNotifyState_SpawnCollider::UAnimNotifyState_SpawnCollider()
{
	bIsNativeBranchingPoint = true;
	bAttached = true;
}

#if WITH_EDITOR
TArray<FName> UAnimNotifyState_SpawnCollider::GetSocketNames() const
{
	TArray<FName> SocketNames;
	SocketNames.Emplace(NAME_None);
	if (const UAnimSequenceBase* Anim = Cast<UAnimSequenceBase>(GetContainingAsset()))
	{
		if (USkeletalMesh* Mesh = Anim->GetPreviewMesh())
		{
			// 소켓 이름 추가
			for (const USkeletalMeshSocket* const& Socket : Mesh->GetActiveSocketList())
			{
				SocketNames.Emplace(Socket->SocketName);
			}
			// 본 이름 추가
			const int32 NumBones = Mesh->GetRefSkeleton().GetNum();
			for (int32 BoneIndex = 0; BoneIndex < NumBones; ++BoneIndex)
			{
				FName BoneName = Mesh->GetRefSkeleton().GetBoneName(BoneIndex);
				SocketNames.Add(BoneName);
			}
		}
	}
	return SocketNames;
}

void UAnimNotifyState_SpawnCollider::ValidateAssociatedAssets()
{
	if (ActorClass == nullptr)
	{
		FMessageLog AssetCheckLog("에셋 검사");
		const FText MessageLooping = FText::FromString(FString::Printf(TEXT("%s ActorClass 가 None 입니다!"), *GetNotifyName()));
		AssetCheckLog.Warning()
		             ->AddToken(FUObjectToken::Create(GetContainingAsset()))
		             ->AddToken(FTextToken::Create(MessageLooping));

		if (GIsEditor)
		{
			AssetCheckLog.Notify(MessageLooping, EMessageSeverity::Warning, /*bForce=*/ true);
		}
	}
}

void UAnimNotifyState_SpawnCollider::DrawInEditor(
	FPrimitiveDrawInterface* PDI,
	USkeletalMeshComponent* MeshComp,
	const UAnimSequenceBase* Animation,
	const FAnimNotifyEvent& NotifyEvent) const
{
	// 엑터 제거 예약. Notify가 시퀸서에서 지워지면 생성한 엑터를 제거
	MeshComp->GetWorld()->GetTimerManager().ClearTimer(RemoveCheckHandle);
	RemoveCheckHandle = MeshComp->GetWorld()->GetTimerManager().SetTimerForNextTick([this, MeshComp]()
	{
		RemoveCheckHandle = MeshComp->GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			if (SpawnedEditorOnly.IsValid())
			{
				SpawnedEditorOnly->Destroy();
			}
		});
	});
	
	// 엑터 생성 위치 시각화
	const FVector DebugPos = bAttached ? MeshComp->GetSocketLocation(SocketName) + Location : Location;
	DrawDebugSphere(MeshComp->GetWorld(), DebugPos, 7.f, 12, NotifyColor, false, DrawDuration);

	// ActorClass가 변경되면 기존 엑터 제거
	if (SpawnedEditorOnly.IsValid() && SpawnedEditorOnly->GetClass() != ActorClass)
	{
		SpawnedEditorOnly->Destroy();
	}

	if (!ActorClass) return;

	// 현재 애니메이션 위치
	float CurTime = 0;
	if (const UAnimMontage* Montage = Cast<UAnimMontage>(Animation))
	{
		CurTime = MeshComp->GetAnimInstance()->Montage_GetPosition(Montage);
	}
	else if (Animation->IsA<UAnimSequence>())
	{
		CurTime = MeshComp->GetSingleNodeInstance()->GetCurrentTime();
	}
	const float StartTime = NotifyEvent.GetTriggerTime();
	const float EndTime = NotifyEvent.GetEndTriggerTime();

	// 노티파이 상태 분기 
	if (CurTime < StartTime || CurTime > EndTime)
	{
		// 엑터 제거
		if (SpawnedEditorOnly.IsValid())
		{
			SpawnedEditorOnly->Destroy();
		}
	}
	else
	{
		// 없으면 생성 후 부착
		if (!SpawnedEditorOnly.IsValid())
		{
			SpawnedEditorOnly = MeshComp->GetWorld()->SpawnActor<AActor>(ActorClass);
		}
		if (SpawnedEditorOnly.IsValid() && bAttached)
		{
			SpawnedEditorOnly->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			SpawnedEditorOnly->SetActorRelativeLocation(Location);
		}
	}
}
#endif

void UAnimNotifyState_SpawnCollider::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{	
	if (!ActorClass || MeshComp->GetNetMode() == NM_Client) return;
	
#if WITH_EDITORONLY_DATA // 노티파이가 트리거된 곳이 PIE/SIE 가 아닌 에디터 프리뷰 씬이고, 이전에 생성한 엑터가 존재하는 경우 패스
	if (!GEditor->IsPlayingSessionInEditor() && SpawnedEditorOnly.IsValid()) return;
#endif
	
	// 콜라이더 생성 및 부착
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (AActor* SpawnedCollider = MeshComp->GetWorld()->SpawnActor<AActor>(ActorClass, SpawnParams))
	{
		if (bAttached)
		{
			SpawnedCollider->SetOwner(MeshComp->GetOwner());
			SpawnedCollider->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			SpawnedCollider->SetActorRelativeLocation(Location);
		}
		// 해당 캐릭터에 키맵으로 콜라이더를 저장해두고, 노티파이가 끝날 때 제거
		if (ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(MeshComp->GetOwner()))
		{
			RPGCharacter->SpawnedColliderMap.Emplace(GetNotifyKey(Animation), SpawnedCollider);
		}
#if WITH_EDITORONLY_DATA // 시퀸서 트랙에서 애니메이션 위치를 임의로 바꾼경우 대응
		if (!GEditor->IsPlayingSessionInEditor())
		{
			if (SpawnedEditorOnly.IsValid())
			{
				SpawnedEditorOnly->Destroy();
			}
			SpawnedEditorOnly = SpawnedCollider;
		}
#endif
	}
	else
	{
		LOG_ERROR("콜라이더 생성 뷸가 (%s >> %s)", *MeshComp->GetOwner()->GetName(), *Animation->GetName());
	}
}

void UAnimNotifyState_SpawnCollider::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (!ActorClass || MeshComp->GetNetMode() == NM_Client) return;

	// 노티파이가 끝났으니 콜라이더 제거
	TObjectPtr<AActor> SpawnedCollider;
	ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(MeshComp->GetOwner());
	if (RPGCharacter && RPGCharacter->SpawnedColliderMap.RemoveAndCopyValue(GetNotifyKey(Animation), SpawnedCollider))
	{
		SpawnedCollider->Destroy();
	}
}
