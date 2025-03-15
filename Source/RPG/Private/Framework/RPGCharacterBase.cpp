// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/RPGCharacterBase.h"
#include "Components/RPGAbilitySystemComponent.h"
#include "Characters/RPGAttributeSet.h"


ARPGCharacterBase::ARPGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComponent");
}

void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// 어빌리티 등록 ~
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
	// ~ 기본 어빌리티 등록

	// 속성 초기화 ~
	AttributeSet = AbilitySystemComponent->GetSet<URPGAttributeSet>();

	if (!DefaultAttributeEffect) return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1.f, EffectContext);
	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	// ~ 속성 초기화
}

void ARPGCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
