// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"

#include "Aura/AuraGameplayTags.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
  UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

  check(AttributeInfo);

  for (const auto& Pair : AS->TagsToAttributes)
  {
    FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
    Info.AttributeValue = Pair.Value.GetNumericValue(AS);
    AttributeInfoDelegate.Broadcast((Info));
  }
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {}
