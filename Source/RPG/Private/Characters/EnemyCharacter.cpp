// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include "Components/Others/RPGAbilitySystemComponent.h"
#include "Gameplay/AttributeSets/RPGAttributeSet.h"
#include "UIs/Others/NPCStatusBar.h"

#include "Components/WidgetComponent.h"
#include "RPG/RPG.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	StatusBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusBar"));
	StatusBarWidget->SetupAttachment(RootComponent);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	const URPGAttributeSet* AttributSet = AbilitySystemComponent->GetSet<URPGAttributeSet>();
	check(AttributSet)
	
	StatusBar = Cast<UNPCStatusBar>(StatusBarWidget->GetWidget());
	if (!StatusBar)
	{
		LOG_CALLINFO_ERROR("StatusBar widget is null");
		return;
	}
	
	StatusBar->UpdateHealthBar(AttributSet->GetHealth(), AttributSet->GetMaxHealth());
	OnHealthChangedDelegate = &AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributSet->GetHealthAttribute());
	OnHealthChangedDelegate->AddUObject(this, &AEnemyCharacter::OnHealthChanged);
}

void AEnemyCharacter::OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (!StatusBar) return;
	
	const URPGAttributeSet* AttributSet = AbilitySystemComponent->GetSet<URPGAttributeSet>();
	StatusBar->UpdateHealthBar(OnAttributeChangeData.NewValue, AttributSet->GetMaxHealth());
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
