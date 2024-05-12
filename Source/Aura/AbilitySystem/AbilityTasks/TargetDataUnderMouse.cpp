// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
  UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
  return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
  const auto PC = Ability->GetCurrentActorInfo()->PlayerController;
  FHitResult CursorHit;
  PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
  ValidData.Broadcast(CursorHit.Location);
}
