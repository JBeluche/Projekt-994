// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994//Useables/Weapons/WeaponSemiAutomatic.h"

#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"

#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

AWeaponSemiAutomatic::AWeaponSemiAutomatic()
{
    WeaponMaxAmmo = 98;
    MagazineMaxAmmo = 7;
    WeaponName = "Default Name";
}

void AWeaponSemiAutomatic::BeginPlay()
{
    Super::BeginPlay();
    CurrentTotalAmmo = WeaponMaxAmmo;
    CurrentMagazineAmmo = MagazineMaxAmmo;
    
}

bool AWeaponSemiAutomatic::Fire(AProjekt994Character *ShootingPlayer)
{
   return Super::Fire(ShootingPlayer);
}

void AWeaponSemiAutomatic::Server_Fire_Implementation(const TArray<FHitResult> &HitResults)
{
    Super::Server_Fire_Implementation(HitResults);
}

int8 AWeaponSemiAutomatic::Reload()
{
    return Super::Reload();
}