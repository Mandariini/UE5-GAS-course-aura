// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
  OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent *AbilitySystemComponent,
                                                const FGameplayEffectSpec &GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
  GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Blue, FString("Effect applied"));
}
