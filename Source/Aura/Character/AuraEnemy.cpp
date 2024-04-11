// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEnemy.h"

void AAuraEnemy::HighlightActor() {
  UE_LOG(LogTemp, Warning, TEXT("HIGHLIGHT"));
  DrawDebugSphere(this->GetWorld(), GetActorLocation(), 20.f, 5, FColor::Red, false, 1.f);
}

void AAuraEnemy::UnHighlightActor() {}
