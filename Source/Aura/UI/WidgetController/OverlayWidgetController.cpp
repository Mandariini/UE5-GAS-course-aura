// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"

#include "Aura/AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
  const auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
  
  OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
  OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}
