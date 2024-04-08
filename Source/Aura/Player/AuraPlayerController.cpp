// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
  bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
  Super::BeginPlay();

  check(AuraContext);

  UEnhancedInputLocalPlayerSubsystem *SubSystem =
    ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
  check(SubSystem);
  SubSystem->AddMappingContext(AuraContext, 0);

  bShowMouseCursor = true;
  DefaultMouseCursor = EMouseCursor::Default;

  FInputModeGameAndUI InputModeData;
  InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
  InputModeData.SetHideCursorDuringCapture(false);
  SetInputMode(InputModeData);
}