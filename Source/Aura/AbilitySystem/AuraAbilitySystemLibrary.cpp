// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Aura/Game/AuraGameModeBase.h"
#include "Aura/Player/AuraPlayerState.h"
#include "Aura/UI/HUD/AuraHUD.h"
#include "Aura/UI/WidgetController/AuraWidgetController.h"
#include "Kismet/GameplayStatics.h"

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

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
  const UObject* WorldContextObject)
{
  if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
  {
    if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
    {
      AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
      UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
      UAttributeSet* AS = PS->GetAttributeSet();
      const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
      return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
    }
  }

  return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                            ECharacterClass CharacterClass, float Level,
                                                            UAbilitySystemComponent* ASC)
{
  AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
  if (!AuraGameModeBase)
  {
    return;
  }

  const AActor* AvatarActor = ASC->GetAvatarActor();

  const auto ClassInfo = AuraGameModeBase->CharacterClassInfo;
  const auto ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

  auto PrimaryAttributesContextHandle = ASC->MakeEffectContext();
  PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
  const auto PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level,
                                                                 PrimaryAttributesContextHandle);
  ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);

  auto SecondaryAttributesContextHandle = ASC->MakeEffectContext();
  SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
  const auto SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level,
                                                                   SecondaryAttributesContextHandle);
  ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);

  auto VitalAttributesContextHandle = ASC->MakeEffectContext();
  VitalAttributesContextHandle.AddSourceObject(AvatarActor);
  const auto VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level,
                                                               VitalAttributesContextHandle);
  ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
  AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
  if (!AuraGameModeBase)
  {
    return;
  }

  const auto ClassInfo = AuraGameModeBase->CharacterClassInfo;
  for (const auto& AbilityClass : ClassInfo->CommonAbilities)
  {
    auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
    ASC->GiveAbility(AbilitySpec);
  }
}
