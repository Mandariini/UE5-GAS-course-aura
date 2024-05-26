// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "Aura/AuraGameplayTags.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"

struct AuraDamageStatics
{
  DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
  DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)

  AuraDamageStatics()
  {
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
  }

  static const AuraDamageStatics& DamageStatics()
  {
    static AuraDamageStatics DamageStatics;
    return DamageStatics;
  }
};

UExecCalc_Damage::UExecCalc_Damage()
{
  RelevantAttributesToCapture.Add(AuraDamageStatics::DamageStatics().ArmorDef);
  RelevantAttributesToCapture.Add(AuraDamageStatics::DamageStatics().BlockChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
  // So, this belongs in a gameplay effect.
  // So if we add a modifier to the gameplay effect, then by setting a custom calculation class, it's really
  // this custom calculation class that determines what's going to happen when we apply that gameplay effect.

  // So here we can decide how to change various attributes and we can capture attributes and we can also
  // get information about the gameplay effect concerning who caused the effect and who the effect is targeting
  // at the time of application.

  const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
  const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

  const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
  const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

  const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

  FAggregatorEvaluateParameters EvaluateParameters;
  EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
  EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

  // Get Damage Set by Caller Magnitude
  float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::GetInstance().Damage);

  // Capture BlockChance on Target and determine if there was a successful Block
  // If Block, halve the damage
  float TargetBlockChance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::DamageStatics().BlockChanceDef,
                                                             EvaluateParameters, TargetBlockChance);
  TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

  const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
  Damage = bBlocked ? Damage / 2.f : Damage;

  const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),
                                                     EGameplayModOp::Additive, Damage);
  OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
