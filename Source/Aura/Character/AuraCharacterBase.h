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
  virtual void Die() override;

  UFUNCTION(NetMulticast, Reliable)
  virtual void MulticastHandleDeath();

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

  /* Dissolve Effects */
  void Dissolve();

  UFUNCTION(BlueprintImplementableEvent)
  void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

  UFUNCTION(BlueprintImplementableEvent)
  void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

private:
  UPROPERTY(EditAnywhere, Category = "Abilities")
  TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

  UPROPERTY(EditAnywhere, Category = "Combat")
  TObjectPtr<UAnimMontage> HitReactMontage;
};
