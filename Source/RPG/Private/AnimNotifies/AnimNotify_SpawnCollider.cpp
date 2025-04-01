// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_SpawnCollider.h"

#include "RPG/RPG.h"


#if WITH_EDITOR
void UAnimNotify_SpawnCollider::DrawInEditor(
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
	
	// ActorClass가 변경되면 기존 엑터 제거
	if (SpawnedEditorOnly.IsValid() && SpawnedEditorOnly->GetClass() != ActorClass)
	{
		SpawnedEditorOnly->Destroy();
	}
	
	if (!ActorClass) return;
	
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
#endif

void UAnimNotify_SpawnCollider::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
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
		SpawnedCollider->SetOwner(MeshComp->GetOwner());
		if (bAttached)
		{
			SpawnedCollider->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			SpawnedCollider->SetActorRelativeLocation(Location);
		}
		// 다음 틱에 바로 삭제
		MeshComp->GetWorld()->GetTimerManager().SetTimerForNextTick([SpawnedCollider]()
		{
			if (IsValid(SpawnedCollider))
			{
				SpawnedCollider->Destroy();
			}
		});
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
	}
	else
	{
		LOG_ERROR("콜라이더 생성 뷸가 (%s >> %s)", *MeshComp->GetOwner()->GetName(), *Animation->GetName());
	}
}