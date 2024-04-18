// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEffectActor.h"

#include "AbilitySystemInterface.h"
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
}

void AAuraEffectActor::OnEndOverlap(AActor *TargetActor)
{
  if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
    ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
  }

  if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
    ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
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
  TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}
