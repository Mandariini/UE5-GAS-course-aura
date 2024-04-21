// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
  OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent *AbilitySystemComponent,
                                                const FGameplayEffectSpec &GameplayEffectSpec,
                                                FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
  FGameplayTagContainer TagContainer;
  GameplayEffectSpec.GetAllAssetTags(TagContainer);
  for (const auto &Tag : TagContainer) {
    // TODO: Broadcast the tag to the Widget Controller
    const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
    GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
  }
}
