// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_SpawnActor.h"

#include "RPG/RPG.h"
#include "Misc/UObjectToken.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Engine/SkeletalMeshSocket.h"

UAnimNotify_SpawnActor::UAnimNotify_SpawnActor()
{
	bIsNativeBranchingPoint = true;
	bAttached = true;
}

#if WITH_EDITOR
TArray<FName> UAnimNotify_SpawnActor::GetSocketNames() const
{
	TArray<FName> SocketNames;
	SocketNames.Emplace(NAME_None);
	if (const UAnimSequenceBase* Anim = Cast<UAnimSequenceBase>(GetContainingAsset()))
	{
		if (USkeletalMesh* Mesh = Anim->GetPreviewMesh())
		{
			// 소켓 이름 추가
			for (const USkeletalMeshSocket* const& Socket : Anim->GetPreviewMesh()->GetActiveSocketList())
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

void UAnimNotify_SpawnActor::ValidateAssociatedAssets()
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

void UAnimNotify_SpawnActor::DrawInEditor(
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
	DrawDebugSphere(MeshComp->GetWorld(), DebugPos, 7.f, 12, NotifyColor, false, 0.05f);

	// ActorClass 가 변경되면 기존 엑터 제거
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

	// 시퀸서에서는 애니메이션 위치를 임의로 State 안팎으로 조정할 수 있기 때문에 그에 따른 내용
	// Notify 활성화 전 / 후 분기
	if (CurTime < NotifyEvent.GetTriggerTime())
	{
		if (SpawnedEditorOnly.IsValid())
		{
			SpawnedEditorOnly->Destroy();
		}
	}
	else
	{
		// 프리뷰 엑터가 없을시 생성 후 부착
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

void UAnimNotify_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!ActorClass || MeshComp->GetNetMode() == NM_Client) return;
	CALLINFO

#if WITH_EDITORONLY_DATA // 노티파이가 트리거된 곳이 PIE/SIE 가 아닌 에디터 프리뷰 씬이고, 이전에 생성한 엑터가 존재하는 경우 패스
	if (!GEditor->IsPlayingSessionInEditor() && SpawnedEditorOnly.IsValid()) return;
#endif

	// 엑터 생성 및 부착
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (AActor* SpawnedCollider = MeshComp->GetWorld()->SpawnActor<AActor>(ActorClass, SpawnParams))
	{
		SpawnedCollider->SetOwner(MeshComp->GetOwner());
		if (bAttached)
		{
			SpawnedCollider->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			SpawnedCollider->SetActorRelativeLocation(Location);
		}
#if WITH_EDITORONLY_DATA // 시퀸서 트랙에서 애니메이션 위치를 임의로 바꾼경우에 대응
		if (!GEditor->IsPlayingSessionInEditor())
		{
			if (SpawnedEditorOnly.IsValid())
			{
				SpawnedEditorOnly->Destroy();
			}
			SpawnedEditorOnly = SpawnedCollider;
		}
#endif
		else
		{
			LOG_ERROR("엑터 생성 뷸가 (%s >> %s)", *MeshComp->GetOwner()->GetName(), *Animation->GetName());
		}
	}
}