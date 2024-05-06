// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemComponent.h"

#include "Aura/AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
  OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::
AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
  for (const auto& AbilityClass : StartupAbilities)
  {
    auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
    // GiveAbility(AbilitySpec);
    GiveAbilityAndActivateOnce(AbilitySpec);
  }
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& GameplayEffectSpec,
                                                FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
  FGameplayTagContainer TagContainer;
  GameplayEffectSpec.GetAllAssetTags(TagContainer);
  EffectAssetTags.Broadcast(TagContainer);
}
