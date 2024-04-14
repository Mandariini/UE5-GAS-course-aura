// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraHUD.h"

#include "Aura/UI/Widget/AuraUserWidget.h"
#include "Aura/UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController *AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams &WCParams)
{
  if (!OverlayWidgetController) {
    OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
    OverlayWidgetController->SetWidgetControllerParams(WCParams);
  }

  return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController *PC, APlayerState *PS, UAbilitySystemComponent *ASC, UAttributeSet *AS)
{
  checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized"));
  checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninitialized"));

  UUserWidget *Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
  OverlayWidget = Cast<UAuraUserWidget>(Widget);

  FWidgetControllerParams WCP(PC, PS, ASC, AS);
  UOverlayWidgetController *WidgetController = GetOverlayWidgetController(WCP);
  OverlayWidget->SetWidgetController(WidgetController);

  Widget->AddToViewport();
}
