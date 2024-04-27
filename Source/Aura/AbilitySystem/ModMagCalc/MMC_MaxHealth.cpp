// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxHealth.h"

#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
  VigorDefinition.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
  VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

  // False = capture attribute when effect is applied (not when effect spec is created)
  VigorDefinition.bSnapshot = false;

  RelevantAttributesToCapture.Add(VigorDefinition);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
  // Gather tags from source and target
  const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
  const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

  FAggregatorEvaluateParameters EvaluationParameters;
  EvaluationParameters.SourceTags = SourceTags;
  EvaluationParameters.TargetTags = TargetTags;

  float Vigor = 0.f;
  if (!GetCapturedAttributeMagnitude(VigorDefinition, Spec, EvaluationParameters, Vigor))
  {
    return 0.f;
  }

  Vigor = FMath::Max<float>(Vigor, 0.f);

  ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
  if (CombatInterface)
  {
    const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
    return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
  }


  return 0.f;
}
