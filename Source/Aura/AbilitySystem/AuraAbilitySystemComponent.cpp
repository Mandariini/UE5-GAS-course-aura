// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemComponent.h"

#include "Aura/AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
  OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

  const auto& GameplayTags = FAuraGameplayTags::GetInstance();
  GEngine->AddOnScreenDebugMessage(
    -1,
    10.f,
    FColor::Orange,
    FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString())
  );
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& GameplayEffectSpec,
                                                FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
  FGameplayTagContainer TagContainer;
  GameplayEffectSpec.GetAllAssetTags(TagContainer);
  EffectAssetTags.Broadcast(TagContainer);
}
