// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraHUD.h"

#include "Aura/UI/Widget/AuraUserWidget.h"

void AAuraHUD::BeginPlay()
{
  Super::BeginPlay();

  UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
  Widget->AddToViewport();
}
