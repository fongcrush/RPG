// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RPGCharacter.h"
#include "Interfaces/Interaction.h"
#include "UIs/RPGHUD.h"

// UE
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ARPGCharacter::ARPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	TurnRateGamepad = 50.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	BaseEyeHeight = 74.f;	

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	InventoryComponent->SetWeightCapacity(200.f);

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.f;
}

// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ARPGCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ARPGCharacter::EndInteract);
		
		EnhancedInputComponent->BindAction(UIToggleAction, ETriggerEvent::Triggered, this, &ARPGCharacter::ToggleMenu);
	}
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<ARPGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ARPGCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
}

void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 상호작용 체크
	if (GetWorld()->TimeSince(LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}

void ARPGCharacter::BeginInteract()
{
	PerformInteractionCheck();

	if (!IsValid(TargetInteractable.GetObject()) || !CurrentInteractable)
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
		GetWorldTimerManager().SetTimer(
			InteractionTimerHandle,
			this,
			&ARPGCharacter::Interact,
			TargetInteractable->GetInteractableData().InteractionDuration,
			false
		);
	}
}

void ARPGCharacter::PerformInteractionCheck()
{
	LastInteractionCheckTime = GetWorld()->GetTimeSeconds();
	
	// 카메라 방향과 캐릭터 전방이 일치하는지
	if (FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector()) <= 0)
	{
		NoInteractableFound();
		return;
	}

	// 트레이싱 설정
	const FVector TraceStart{GetPawnViewLocation()};
	const FVector TraceEnd{TraceStart + (InteractionCheckDistance * GetViewRotation().Vector())};
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	FHitResult TraceHit; // 라인 트레이싱
	if ( GetWorld()->LineTraceSingleByChannel(TraceHit,	TraceStart,	TraceEnd, ECC_Visibility, CollisionQueryParams))
	{
		FoundInteractable(TraceHit.GetActor());
	}
}

void ARPGCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(InteractionTimerHandle);
	}
	
	if (!CurrentInteractable)
		return;
	
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndFocus();
		EndInteract();
	}
	HUD->HideInteractionWidget();
	
	CurrentInteractable = nullptr;
	TargetInteractable = nullptr;
}

void ARPGCharacter::FoundInteractable(AActor* Target)
{
	// 같은 오브젝트 거나 상호작용 오브젝트가 아닌 경우
	if (Target == CurrentInteractable || !Target->Implements<UInteraction>())
	{
		return;
	}
	// 이미 상호작용 중
	if (IsInteracting())
	{
		EndInteract();
	}
	
	if (CurrentInteractable)
	{
		TargetInteractable = CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	CurrentInteractable = Target;
	TargetInteractable = Target;

	UpdateInteractionWidget();
	TargetInteractable->BeginFocus();
}

void ARPGCharacter::UpdateInteractionWidget() const
{
	HUD->UpdateInteractionWidget(TargetInteractable->GetInteractableData());
}

void ARPGCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(InteractionTimerHandle);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void ARPGCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(InteractionTimerHandle);
	
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

void ARPGCharacter::Move(const FInputActionValue& Value)
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	const FVector2D MoveVector = Value.Get<FVector2D>();
	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}

void ARPGCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}