// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UAuraInputConfig;
class UInputAction;
class UInputMappingContext;
class IEnemyInterface;
struct FInputActionValue;

/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
  GENERATED_BODY()

public:
  AAuraPlayerController();

  virtual void PlayerTick(float DeltaTime) override;

protected:
  virtual void BeginPlay() override;
  virtual void SetupInputComponent() override;

private:
  void Move(const FInputActionValue& InputActionValue);
  void CursorTrace();

private:
  UPROPERTY(EditAnywhere, Category = "Input")
  TObjectPtr<UInputMappingContext> AuraContext;

  UPROPERTY(EditAnywhere, Category = "Input")
  TObjectPtr<UInputAction> MoveAction;

  IEnemyInterface* LastActor;
  IEnemyInterface* ThisActor;
  FHitResult CursorHit;

  void AbilityInputTagPressed(FGameplayTag InputTag);
  void AbilityInputTagReleased(FGameplayTag InputTag);
  void AbilityInputTagHeld(FGameplayTag InputTag);

  UPROPERTY(EditDefaultsOnly, Category="Input")
  TObjectPtr<UAuraInputConfig> InputConfig;

  UPROPERTY()
  TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

  UAuraAbilitySystemComponent* GetASC();

  // Point click move stuff
  FVector CachedDestination = FVector::ZeroVector;
  float FollowTime = 0.f;
  float ShortPressThreshold = 0.3f;
  bool bAutoRunning = false;
  bool bTargeting = false;

  UPROPERTY(EditDefaultsOnly)
  float AutoRunAcceptanceRadius = 50.f;

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<USplineComponent> Spline;
  void AutoRun();
};
