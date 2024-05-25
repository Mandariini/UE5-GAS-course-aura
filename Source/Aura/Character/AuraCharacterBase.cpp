// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
  PrimaryActorTick.bCanEverTick = false;

  GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
  GetCapsuleComponent()->SetGenerateOverlapEvents(false);

  GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
  GetMesh()->SetCollisionResponseToChannel(ECC_PROJECTILE, ECR_Overlap);
  GetMesh()->SetGenerateOverlapEvents(true);

  Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
  Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
  Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
  return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
  return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
  Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
  MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
  Weapon->SetSimulatePhysics(true);
  Weapon->SetEnableGravity(true);
  Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

  GetMesh()->SetSimulatePhysics(true);
  GetMesh()->SetEnableGravity(true);
  GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
  GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // Walls, floors etc.

  GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

  Dissolve();
}

void AAuraCharacterBase::BeginPlay()
{
  Super::BeginPlay();
}

void AAuraCharacterBase::InitializeAbilityActorInfo() {}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
  check(Weapon);
  return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
  check(IsValid(GetAbilitySystemComponent()));
  check(GameplayEffectClass);

  FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
  EffectContextHandle.AddSourceObject(this);

  const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
    GameplayEffectClass, Level,
    EffectContextHandle);

  GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
  ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
  ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
  ApplyEffectToSelf(DefaultVitalAttributes, 1.0f);
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
  if (!HasAuthority())
  {
    return;
  }

  UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
  AuraASC->AddCharacterAbilities(StartupAbilities);
}

void AAuraCharacterBase::Dissolve()
{
  if (IsValid(DissolveMaterialInstance))
  {
    UMaterialInstanceDynamic* DynamicMaterialInstance =
      UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
    GetMesh()->SetMaterial(0, DynamicMaterialInstance);
    StartDissolveTimeline(DynamicMaterialInstance);
  }

  if (IsValid(WeaponDissolveMaterialInstance))
  {
    UMaterialInstanceDynamic* DynamicMaterialInstance =
      UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
    Weapon->SetMaterial(0, DynamicMaterialInstance);
    StartWeaponDissolveTimeline(DynamicMaterialInstance);
  }
}
