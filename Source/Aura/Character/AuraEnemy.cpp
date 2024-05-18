// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEnemy.h"

#include "Aura/Aura.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/UI/Widget/AuraUserWidget.h"
#include "Components/WidgetComponent.h"

AAuraEnemy::AAuraEnemy()
{
  GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

  AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
  AbilitySystemComponent->SetIsReplicated(true);
  AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

  AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

  HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
  HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::HighlightActor()
{
  GetMesh()->SetRenderCustomDepth(true);
  GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
  Weapon->SetRenderCustomDepth(true);
  Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
  GetMesh()->SetRenderCustomDepth(false);
  Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
  return Level;
}

void AAuraEnemy::BeginPlay()
{
  Super::BeginPlay();
  InitializeAbilityActorInfo();

  if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
  {
    AuraUserWidget->SetWidgetController(this);
  }


  if (const auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
  {
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
      [this](const FOnAttributeChangeData& Data)
      {
        OnHealthChanged.Broadcast(Data.NewValue);
      });

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).
                            AddLambda(
                              [this](const FOnAttributeChangeData& Data)
                              {
                                OnMaxHealthChanged.Broadcast(Data.NewValue);
                              });

    OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
  }
}

void AAuraEnemy::InitializeAbilityActorInfo()
{
  AbilitySystemComponent->InitAbilityActorInfo(this, this);
  Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

  InitializeDefaultAttributes();
}
