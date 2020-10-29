// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994//Useables/Weapon1911.h"

AWeapon1911::AWeapon1911()
{
    WeaponMaxAmmo = 98;
    MagazineMaxAmmo = 7;
    WeaponName = "1911";

    CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeapon1911::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Shooting 1991"));
}

void AWeapon1911::Reload()
{

}
