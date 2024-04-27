// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
  PrimaryActorTick.bCanEverTick = false;

  Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
  Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
  Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
  return AbilitySystemComponent;
}

void AAuraCharacterBase::BeginPlay()
{
  Super::BeginPlay();
}

void AAuraCharacterBase::InitializeAbilityActorInfo() {}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
  check(IsValid(GetAbilitySystemComponent()));
  check(GameplayEffectClass);

  const FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
  const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
    GameplayEffectClass, Level,
    EffectContextHandle);

  GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
  ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
  ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
}
