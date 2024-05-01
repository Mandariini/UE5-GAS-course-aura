// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"

#include "Aura/AuraGameplayTags.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
  UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

  check(AttributeInfo);

  FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(
    FAuraGameplayTags::GetInstance().Attributes_Primary_Strength, true);
  Info.AttributeValue = AS->GetStrength();

  AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {}
