// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"

UExecCalc_Damage::UExecCalc_Damage() {}

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
}
