// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Aura/AuraGameplayTags.h"
#include "GameFramework/Character.h"

UAuraAttributeSet::UAuraAttributeSet()
{
  const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::GetInstance();

  // Primary Attributes
  TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute());

  // Secondary Attributes
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute());
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute());
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  // Primary Attributes
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

  // Secondary Attributes
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

  // Vital Attributes  
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
  Super::PreAttributeChange(Attribute, NewValue);
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
    UE_LOG(LogTemp, Warning, TEXT("Changed health on %s to %f"), *Props.Target.Character.GetName(), GetHealth());
  }
  if (Data.EvaluatedData.Attribute == GetManaAttribute())
  {
    SetMana(FMath::Clamp(GetMaxMana(), 0, GetMaxMana()));
  }
  if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
  {
    const float LocalIncomingDamage = GetIncomingDamage();
    SetIncomingDamage(0.f);
    if (LocalIncomingDamage > 0.f)
    {
      const float NewHealth = GetHealth() - LocalIncomingDamage;
      SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

      const bool bFatal = NewHealth <= 0.f;
      if (!bFatal)
      {
        Props.Target.ASC->TryActivateAbilitiesByTag(
          FGameplayTagContainer(FAuraGameplayTags::GetInstance().Effects_HitReact));
      }
    }
  }
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
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
