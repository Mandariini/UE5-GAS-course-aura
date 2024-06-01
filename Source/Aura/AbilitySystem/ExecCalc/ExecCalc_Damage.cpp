// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "Aura/AuraGameplayTags.h"
#include "Aura/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AbilitySystem/Data/CharacterClassInfo.h"
#include "Aura/Interaction/CombatInterface.h"

struct AuraDamageStatics
{
  DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
  DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
  DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
  DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
  DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
  DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

  AuraDamageStatics()
  {
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Source, false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
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
  RelevantAttributesToCapture.Add(AuraDamageStatics::DamageStatics().ArmorPenetrationDef);
  RelevantAttributesToCapture.Add(AuraDamageStatics::DamageStatics().BlockChanceDef);
  RelevantAttributesToCapture.Add(AuraDamageStatics::DamageStatics().CriticalHitChanceDef);
  RelevantAttributesToCapture.Add(AuraDamageStatics::DamageStatics().CriticalHitResistanceDef);
  RelevantAttributesToCapture.Add(AuraDamageStatics::DamageStatics().CriticalHitDamageDef);
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

  AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
  AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
  ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
  ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

  const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

  FAggregatorEvaluateParameters EvaluateParameters;
  EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
  EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

  // Get Damage Set by Caller Magnitude
  float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::GetInstance().Damage);

  // Capture BlockChance on Target and determine if there was a successful Block
  float TargetBlockChance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::DamageStatics().BlockChanceDef,
                                                             EvaluateParameters, TargetBlockChance);
  TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

  const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

  // If Block, halve the damage
  Damage = bBlocked ? Damage / 2.f : Damage;

  float TargetArmor = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::DamageStatics().ArmorDef,
                                                             EvaluateParameters, TargetArmor);
  TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

  float SourceArmorPenetration = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::DamageStatics().ArmorPenetrationDef,
                                                             EvaluateParameters, SourceArmorPenetration);
  SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

  const auto CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

  const auto ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
    FName("ArmorPenetration"), FString());
  const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());

  // ArmorPenetration ignores a percentage of the Target's Armor.
  const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

  const auto EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
    FName("EffectiveArmor"), FString());
  const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

  // Armor ignores a percentage of incoming damage
  Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

  float SourceCriticalHitChance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::DamageStatics().CriticalHitChanceDef,
                                                             EvaluateParameters, SourceCriticalHitChance);
  SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

  float TargetCriticalHitResistance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
    AuraDamageStatics::DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
  TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

  float SourceCriticalHitDamage = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::DamageStatics().CriticalHitDamageDef,
                                                             EvaluateParameters, SourceCriticalHitDamage);
  SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

  const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
    FName("CriticalHitResistance"), FString());
  const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(
    TargetCombatInterface->GetPlayerLevel());

  // Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
  const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance *
    CriticalHitResistanceCoefficient;
  const bool bCriticalHit = FMath::FRandRange(1.f, 100.f) < EffectiveCriticalHitChance;

  // Double damage plus a bonus if critical hit
  Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

  const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),
                                                     EGameplayModOp::Additive, Damage);
  OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
