// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraProjectileSpell.h"

#include "Aura/Actor/AuraProjectile.h"
#include "Aura/Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
  Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

  const auto bIsServer = HasAuthority(&ActivationInfo);
  if (!bIsServer)
  {
    return;
  }

  ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
  if (CombatInterface)
  {
    const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

    FTransform SpawnTransform;
    SpawnTransform.SetLocation(SocketLocation);
    // TODO: Rotation

    const auto Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
                                                                            SpawnTransform,
                                                                            GetOwningActorFromActorInfo(),
                                                                            Cast<APawn>(GetOwningActorFromActorInfo()),
                                                                            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    check(Projectile);

    // TODO: Give the Projectile a Gameplay Effect Spec for causing Damage.

    Projectile->FinishSpawning(SpawnTransform);
  }
}
