// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aura/Interaction/EnemyInterface.h"
#include "AuraCharacterBase.h"
#include "AuraEnemy.generated.h"

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

protected:
  virtual void BeginPlay() override;

  virtual void InitializeAbilityActorInfo() override;
};
