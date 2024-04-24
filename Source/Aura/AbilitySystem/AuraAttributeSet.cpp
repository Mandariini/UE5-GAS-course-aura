// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"

UAuraAttributeSet::UAuraAttributeSet()
{
  InitHealth(50.f);
  InitMaxHealth(100.f);
  InitMana(50.f);
  InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
  Super::PreAttributeChange(Attribute, NewValue);

  // All this does is clamp what's returned from querying the modifier.
  if (Attribute == GetHealthAttribute())
  {
    NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
  }
  if (Attribute == GetManaAttribute())
  {
    NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
  }
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
  Super::PostGameplayEffectExecute(Data);

  FEffectProperties Props;
  SetEffectProperties(Data, Props);

  // Here we actually set the value to a clamped value.
  if (Data.EvaluatedData.Attribute == GetHealthAttribute())
  {
    SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
  }
  if (Data.EvaluatedData.Attribute == GetManaAttribute())
  {
    SetMana(FMath::Clamp(GetMaxMana(), 0, GetMaxMana()));
  }
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
  // Source = cause of the effect, Target = owner of this AS

  Props.EffectContextHandle = Data.EffectSpec.GetContext();
  Props.Source.ASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

  if (IsValid(Props.Source.ASC) && Props.Source.ASC->AbilityActorInfo.IsValid() && Props.Source.ASC->AbilityActorInfo->
    AvatarActor.IsValid())
  {
    Props.Source.AvatarActor = Props.Source.ASC->AbilityActorInfo->AvatarActor.Get();

    // Try to get from ability actor info
    Props.Source.Controller = Props.Source.ASC->AbilityActorInfo->PlayerController.Get();
    if (!Props.Source.Controller && Props.Source.AvatarActor)
    {
      // Try to get from actor instead
      if (const APawn* Pawn = Cast<APawn>(Props.Source.AvatarActor))
      {
        Props.Source.Controller = Pawn->GetController();
      }
    }

    if (Props.Source.Controller)
    {
      Props.Source.Character = Cast<ACharacter>(Props.Source.Controller->GetPawn());
    }
  }

  if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
  {
    Props.Target.AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
    Props.Target.Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
    Props.Target.Character = Cast<ACharacter>(Props.Target.AvatarActor);
    Props.Target.ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.Target.AvatarActor);
  }
}
