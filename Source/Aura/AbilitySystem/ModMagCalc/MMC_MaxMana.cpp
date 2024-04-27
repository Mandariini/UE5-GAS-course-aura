// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxMana.h"

#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
  IntelligenceDefinition.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
  IntelligenceDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  IntelligenceDefinition.bSnapshot = false;

  RelevantAttributesToCapture.Add(IntelligenceDefinition);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
  // Gather tags from source and target
  const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
  const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

  FAggregatorEvaluateParameters EvaluationParameters;
  EvaluationParameters.SourceTags = SourceTags;
  EvaluationParameters.TargetTags = TargetTags;

  float Intelligence = 0.f;
  if (!GetCapturedAttributeMagnitude(IntelligenceDefinition, Spec, EvaluationParameters, Intelligence))
  {
    return 0.f;
  }

  Intelligence = FMath::Max<float>(Intelligence, 0.f);

  ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
  if (CombatInterface)
  {
    const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
    return 80.f + 3.0f * Intelligence + 10.f * PlayerLevel;
  }

  return 0.f;
}
