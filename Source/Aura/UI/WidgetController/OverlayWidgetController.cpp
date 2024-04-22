// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"

#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
  const auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

  OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
  OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
  OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
  OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
  const auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
      OnHealthChanged.Broadcast(Data.NewValue);
    });

  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
      OnMaxHealthChanged.Broadcast(Data.NewValue);
    });

  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
      OnManaChanged.Broadcast(Data.NewValue);
    });

  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
      OnMaxManaChanged.Broadcast(Data.NewValue);
    });

  Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
    [this](const FGameplayTagContainer& AssetTags)
    {
      for (const auto& Tag : AssetTags)
      {
        const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
        if (Tag.MatchesTag(MessageTag))
        {
          const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
          MessageWidgetRowDelegate.Broadcast(*Row);
        }
      }
    });
}
