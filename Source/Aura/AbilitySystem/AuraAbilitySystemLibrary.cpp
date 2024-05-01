// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemLibrary.h"

#include "Aura/Player/AuraPlayerState.h"
#include "Aura/UI/HUD/AuraHUD.h"
#include "Aura/UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
  if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
  {
    if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
    {
      AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
      UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
      UAttributeSet* AS = PS->GetAttributeSet();
      const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
      return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
    }
  }

  return nullptr;
}
