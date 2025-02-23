// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractorComponent.h"

#include "Interfaces/Interaction.h"
#include "UIs/RPGHUD.h"


// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.f;
}

// Called when the game starts
void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = Cast<ARPGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	ParentPawn = Cast<APawn>(GetOwner());
	if (!ParentPawn)
	{
		DestroyComponent();
	}
}

// Called every frame
void UInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 상호작용 체크
	if (GetWorld()->TimeSince(LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}

void UInteractorComponent::PerformInteractionCheck()
{
	LastInteractionCheckTime = GetWorld()->GetTimeSeconds();
	
	// 카메라 방향과 캐릭터 전방이 일치하는지
	if (FVector::DotProduct(ParentPawn->GetActorForwardVector(), ParentPawn->GetViewRotation().Vector()) <= 0)
	{
		NoInteractableFound();
		return;
	}

	// 라인 트레이싱 설정
	const FVector TraceStart{ParentPawn->GetPawnViewLocation()};
	const FVector TraceEnd{TraceStart + (InteractionCheckDistance * ParentPawn->GetViewRotation().Vector())};
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 1.0f);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	
	FHitResult TraceHit; // 라인 트레이싱
	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams))
	{
		FoundInteractable(TraceHit.GetActor());
	}
}

void UInteractorComponent::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(InteractionTimerHandle);
	}
	
	if (!TargetInteractable)
		return;
	
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndFocus();
		EndInteract();
	}
	HUD->HideInteractionWidget();
	
	TargetInteractable = nullptr;
}

void UInteractorComponent::FoundInteractable(AActor* Target)
{
	const AActor* CurrentInteractable = TargetInteractable ? Cast<AActor>(TargetInteractable.GetObject()) : nullptr;
	// 상호작용 오브젝트가 아니거나 같은 오브젝트인 경우 패스
	if (!Target->Implements<UInteraction>() || Target == CurrentInteractable)
	{
		return;
	}
	// 상호작용 하던 중이었다면 해당 상호작용 종료
	if (IsInteracting())
	{
		EndInteract();
	}
	// 포커싱하던 다른 물체의 포커싱 종료
	if (IsValid(CurrentInteractable))
	{
		TargetInteractable->EndFocus();
		HUD->HideInteractionWidget();
	}
	TargetInteractable = Target;
	TargetInteractable->BeginFocus();
	HUD->UpdateInteractionWidget(TargetInteractable->GetInteractableData());
}

void UInteractorComponent::BeginInteract()
{
	if (!IsValid(TargetInteractable.GetObject()))
	{
		return;
	}
	
	TargetInteractable->BeginInteract();

	if (FMath::IsNearlyZero(TargetInteractable->GetInteractableData().InteractionDuration))
	{
		Interact();
	}
	else
	{
		GetOwner()->GetWorldTimerManager().SetTimer(
			InteractionTimerHandle,
			this,
			&UInteractorComponent::Interact,
			TargetInteractable->GetInteractableData().InteractionDuration,
			false
		);
	}
}

void UInteractorComponent::Interact()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(InteractionTimerHandle);
	
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(ParentPawn);
	}
}

void UInteractorComponent::EndInteract()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(InteractionTimerHandle);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}