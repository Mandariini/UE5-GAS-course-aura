// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"

#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
  UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

  for (const auto& Pair : AS->TagsToAttributes)
  {
    BroadcastAttributeInfo(Pair.Key, Pair.Value);
  }
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
  UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

  for (const auto& Pair : AS->TagsToAttributes)
  {
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
      [Pair, this](const FOnAttributeChangeData& Data)
      {
        BroadcastAttributeInfo(Pair.Key, Pair.Value);
      });
  }
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
  check(AttributeInfo);

  FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
  Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
  AttributeInfoDelegate.Broadcast(Info);
}
