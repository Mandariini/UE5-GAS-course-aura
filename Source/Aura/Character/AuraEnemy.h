// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aura/Interaction/EnemyInterface.h"
#include "AuraCharacterBase.h"
#include "Aura/AbilitySystem/Data/CharacterClassInfo.h"
#include "Aura/UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

/**
 *
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
  GENERATED_BODY()

public:
  AAuraEnemy();

  /* Enemy Interface */
  virtual void HighlightActor() override;
  virtual void UnHighlightActor() override;
  /* Enemy Interface */

  /* Combat Interface */
  virtual int32 GetPlayerLevel() override;
  virtual void Die() override;
  /* Combat Interface */

  UPROPERTY(BlueprintAssignable)
  FOnAttributeChangedSignature OnHealthChanged;

  UPROPERTY(BlueprintAssignable)
  FOnAttributeChangedSignature OnMaxHealthChanged;

  void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

  UPROPERTY(BlueprintReadOnly, Category = "Combat")
  bool bHitReacting = false;

  UPROPERTY(BlueprintReadOnly, Category = "Combat")
  float BaseWalkSpeed = 250.f;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
  float LifeSpan = 5.f;

protected:
  virtual void BeginPlay() override;
  virtual void InitializeAbilityActorInfo() override;
  virtual void InitializeDefaultAttributes() const override;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
  int32 Level = 1;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
  ECharacterClass CharacterClass = ECharacterClass::DefaultClass;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UWidgetComponent> HealthBar;
};
