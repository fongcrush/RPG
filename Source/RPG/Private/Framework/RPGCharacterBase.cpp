// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RPGCharacterBase.h"
#include "Characters/RPGAttributeSet.h"
#include "Components/RPGAbilitySystemComponent.h"


ARPGCharacterBase::ARPGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComponent");
	//AttributeSet = CreateDefaultSubobject<URPGAttributeSet>("AttributeSet");
}

void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// 기본 어빌리티 등록~
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
	}
	// ~기본 어빌리티 등록

	// 기본 속성 초기화~
	if (!DefaultAttributeEffect) return;
	
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1.f, EffectContext);
	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	// ~기본 속성 초기화
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}