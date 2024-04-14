// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Aura/Interaction/EnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
  bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
  Super::PlayerTick(DeltaTime);

  CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
  Super::BeginPlay();

  check(AuraContext);

  UEnhancedInputLocalPlayerSubsystem *SubSystem =
    ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
  if (SubSystem) {
    SubSystem->AddMappingContext(AuraContext, 0);
  }

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

void AAuraPlayerController::CursorTrace()
{
  FHitResult CursorHit;
  const auto bHitOccured = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

  if (!bHitOccured || !CursorHit.bBlockingHit) {
    return;
  }

  LastActor = ThisActor;
  ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

  if (ThisActor != LastActor) {
    UE_LOG(LogTemp, Warning, TEXT("2"));
    if (LastActor) {
      LastActor->UnHighlightActor();
    }

    if (ThisActor) {
      ThisActor->HighlightActor();
    }
  }
}
