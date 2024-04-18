// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
  PrimaryActorTick.bCanEverTick = false;

  SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
  Super::BeginPlay();
}

void AAuraEffectActor::OnOverlap(AActor *TargetActor)
{
  if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
    ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
  }

  if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
    ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
  }

  if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
    ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
  }
}

void AAuraEffectActor::OnEndOverlap(AActor *TargetActor)
{
  if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
    ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
  }

  if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
    ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
  }

  if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
    ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
  }
  if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
    UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (!IsValid(TargetASC)) {
      return;
    }

    for (auto Iterator = ActiveEffectHandles.CreateIterator(); Iterator; ++Iterator) {
      if (TargetASC == Iterator->Value) {
        TargetASC->RemoveActiveGameplayEffect(Iterator->Key, 1);
      }
      Iterator.RemoveCurrent();
    }
  }
}

void AAuraEffectActor::ApplyEffectToTarget(AActor *TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
  UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
  if (!TargetASC) {
    return;
  }

  check(GameplayEffectClass);
  FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
  EffectContextHandle.AddSourceObject(this);
  const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);
  const FActiveGameplayEffectHandle ActiveHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);

  const bool bIsInfinite = EffectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
  if (bIsInfinite && InfiniteEffectRemovalPolicy != EEffectRemovalPolicy::DoNotRemove) {
    ActiveEffectHandles.Add(ActiveHandle, TargetASC);
  }
}
