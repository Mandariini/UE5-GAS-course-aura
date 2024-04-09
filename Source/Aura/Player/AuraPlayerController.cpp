// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraPlayerController.h"

#include "EnhancedInputComponent.h"
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

void AAuraPlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();

  UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

  EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue &InputActionValue)
{
  const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

  // Control looks forward
  const FRotator Rotation = GetControlRotation();
  const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // Only interested in Yaw

  // Get forward vector, parallel to the ground
  const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

  if (APawn *ControlledPawn = GetPawn<APawn>()) {
    ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
    ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
  }
}
