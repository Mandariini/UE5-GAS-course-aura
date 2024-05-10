// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "Aura/AuraGameplayTags.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Input/AuraInputComponent.h"

#include "Aura/Interaction/EnemyInterface.h"
#include "Components/SplineComponent.h"


AAuraPlayerController::AAuraPlayerController()
{
  bReplicates = true;

  Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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

  UEnhancedInputLocalPlayerSubsystem* SubSystem =
    ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
  if (SubSystem)
  {
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

  UAuraInputComponent* EnhancedInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

  EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
  EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
                                             &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
  const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

  // Control looks forward
  const FRotator Rotation = GetControlRotation();
  const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // Only interested in Yaw

  // Get forward vector, parallel to the ground
  const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

  if (APawn* ControlledPawn = GetPawn<APawn>())
  {
    ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
    ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
  }
}

void AAuraPlayerController::CursorTrace()
{
  FHitResult CursorHit;
  const auto bHitOccured = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

  if (!bHitOccured || !CursorHit.bBlockingHit)
  {
    return;
  }

  LastActor = ThisActor;
  ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

  if (ThisActor != LastActor)
  {
    if (LastActor)
    {
      LastActor->UnHighlightActor();
    }

    if (ThisActor)
    {
      ThisActor->HighlightActor();
    }
  }
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
  if (InputTag.MatchesTagExact(FAuraGameplayTags::GetInstance().InputTag_LMB))
  {
    bTargeting = ThisActor ? true : false;
    bAutoRunning = false;
  }
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
  if (!GetASC())
  {
    return;
  }
  GetASC()->AbilityInputTagHeld(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
  if (!InputTag.MatchesTagExact(FAuraGameplayTags::GetInstance().InputTag_LMB))
  {
    if (GetASC())
    {
      GetASC()->AbilityInputTagHeld(InputTag);
    }
    return;
  }

  if (bTargeting)
  {
    if (GetASC())
    {
      GetASC()->AbilityInputTagHeld(InputTag);
    }
  }
  else
  {
    FollowTime += GetWorld()->GetDeltaSeconds();

    FHitResult HitResult;
    if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
    {
      CachedDestination = HitResult.ImpactPoint;
    }

    if (APawn* ControlledPawn = GetPawn())
    {
      const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
      ControlledPawn->AddMovementInput(WorldDirection);
    }
  }
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
  if (!AuraAbilitySystemComponent)
  {
    AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
      UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
  }

  return AuraAbilitySystemComponent;
}
