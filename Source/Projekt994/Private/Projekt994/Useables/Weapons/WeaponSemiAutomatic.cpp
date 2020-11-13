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

