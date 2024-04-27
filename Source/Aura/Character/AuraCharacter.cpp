// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Player/AuraPlayerController.h"
#include "Aura/Player/AuraPlayerState.h"
#include "Aura/UI/HUD/AuraHUD.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
  GetCharacterMovement()->bOrientRotationToMovement = true;
  GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
  GetCharacterMovement()->bConstrainToPlane = true;
  GetCharacterMovement()->bSnapToPlaneAtStart = true;

  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;
  bUseControllerRotationYaw = false;
}

int32 AAuraCharacter::GetPlayerLevel()
{
  const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
  check(AuraPlayerState);

  return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
  Super::PossessedBy(NewController);

  // Init ability actor info for the server
  InitializeAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
  Super::OnRep_PlayerState();

  // Init ability actor info for the client
  InitializeAbilityActorInfo();
}

void AAuraCharacter::InitializeAbilityActorInfo()
{
  AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
  check(AuraPlayerState);
  AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
  Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

  AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
  AttributeSet = AuraPlayerState->GetAttributeSet();

  if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
  {
    if (AAuraHUD* HUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
    {
      HUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
    }
  }

  InitializeDefaultAttributes();
}
