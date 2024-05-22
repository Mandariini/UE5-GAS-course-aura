// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Aura/Interaction/CombatInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
  GENERATED_BODY()

public:
  AAuraCharacterBase();

  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

  UAttributeSet* GetAttributeSet() const
  {
    return AttributeSet;
  };

  virtual UAnimMontage* GetHitReactMontage_Implementation() override;

protected:
  virtual void BeginPlay() override;

  virtual void InitializeAbilityActorInfo();

  UPROPERTY(EditAnywhere, Category = "Combat")
  TObjectPtr<USkeletalMeshComponent> Weapon;

  UPROPERTY(EditAnywhere, Category = "Combat")
  FName WeaponTipSocketName;

  virtual FVector GetCombatSocketLocation() override;

  UPROPERTY()
  TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

  UPROPERTY()
  TObjectPtr<UAttributeSet> AttributeSet;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
  TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
  TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
  TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

  void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
  virtual void InitializeDefaultAttributes() const;

  void AddCharacterAbilities() const;

private:
  UPROPERTY(EditAnywhere, Category = "Abilities")
  TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

  UPROPERTY(EditAnywhere, Category = "Combat")
  TObjectPtr<UAnimMontage> HitReactMontage;
};
