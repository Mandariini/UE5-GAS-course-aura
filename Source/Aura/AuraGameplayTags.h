// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * @brief Singleton containing native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
  static const FAuraGameplayTags& GetInstance() { return GameplayTags; }

  static void InitializeNativeGameplayTags();

protected:

private:
  static FAuraGameplayTags GameplayTags;
};
