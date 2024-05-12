// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
  UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
  return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
  const bool bIsLocallyController = Ability->GetCurrentActorInfo()->IsLocallyControlled();
  if (bIsLocallyController)
  {
    SendMouseCursorData();
  }
  else
  {
    // TODO: We are on the server, so listen for target data.
  }
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
  FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());

  const auto PC = Ability->GetCurrentActorInfo()->PlayerController;
  FHitResult CursorHit;
  PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

  auto Data = new FGameplayAbilityTargetData_SingleTargetHit();
  Data->HitResult = CursorHit;

  FGameplayAbilityTargetDataHandle DataHandle;
  DataHandle.Add(Data);

  // To server
  AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
                                                        GetActivationPredictionKey(),
                                                        DataHandle,
                                                        FGameplayTag(),
                                                        AbilitySystemComponent->ScopedPredictionKey);

  // To locally
  if (ShouldBroadcastAbilityTaskDelegates())
  {
    ValidData.Broadcast(DataHandle);
  }
}
