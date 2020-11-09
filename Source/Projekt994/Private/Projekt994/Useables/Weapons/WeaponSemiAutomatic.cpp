// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994//Useables/Weapons/WeaponSemiAutomatic.h"

#include "Components/SkeletalMeshComponent.h"
#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AWeaponSemiAutomatic::AWeaponSemiAutomatic()
{
    WeaponMaxAmmo = 98;
    MagazineMaxAmmo = 7;
    WeaponName = "Default Name";

    CurrentTotalAmmo = WeaponMaxAmmo;
    CurrentMagazineAmmo = MagazineMaxAmmo;
}

bool AWeaponSemiAutomatic::Fire(AProjekt994Character *ShootingPlayer)
{

    if (CurrentMagazineAmmo > 0)
    {
        Super::Fire(ShootingPlayer);

        if (FireAnimation)
        {
            WeaponMesh->PlayAnimation(FireAnimation, false);
        }

        TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);

        if (HitResults.Num() > 0)
        {
            for (FHitResult &Result : HitResults)
            {
                FString HitBone = Result.BoneName.ToString();
                if (AActor *HitActor = Result.GetActor())
                {
                    if (AZombieBase *Zombie = Cast<AZombieBase>(HitActor))
                    {
                        Zombie->Hit(ShootingPlayer, Result);
                    }
                }
            }
        }
        if (!GetWorld()->IsServer())
        {
            Server_Fire(HitResults);
        }

        return true;
    }

    return false;
}

void AWeaponSemiAutomatic::Server_Fire_Implementation(const TArray<FHitResult> &HitResults)
{
    if (CurrentMagazineAmmo > 0)
    {
        Super::Server_Fire_Implementation(HitResults);

        if (FireAnimation)
        {
            WeaponMesh->PlayAnimation(FireAnimation, false);
        }

        if (HitResults.Num() > 0)
        {

            for (FHitResult Result : HitResults)
            {
                if (AActor *HitActor = Result.GetActor())
                {
                    if (AZombieBase *Zombie = Cast<AZombieBase>(HitActor))
                    {
                        if (AProjekt994Character *Player = Cast<AProjekt994Character>(GetOwner()))
                        {
                            Zombie->Hit(Player, Result);
                        }
                    }
                }
            }
        }
    }
}

bool AWeaponSemiAutomatic::Reload()
{
    if (CurrentTotalAmmo > 0 && CurrentMagazineAmmo != MagazineMaxAmmo)
    {
        if (ReloadAnimation)
        {
            WeaponMesh->PlayAnimation(ReloadAnimation, false);
        }
        UE_LOG(LogTemp, Error, TEXT("Current total ammo: %d"), CurrentTotalAmmo);


        int Difference = MagazineMaxAmmo - CurrentMagazineAmmo;
        if (CurrentTotalAmmo - Difference >= 0)
        {
            CurrentTotalAmmo -= Difference;
            CurrentMagazineAmmo = MagazineMaxAmmo;
        }
        else
        {
            CurrentMagazineAmmo += CurrentTotalAmmo;
            CurrentTotalAmmo = 0;
        }
        
        UE_LOG(LogTemp, Error, TEXT("Current total ammo: %d"), CurrentTotalAmmo);
        return true;
    }
    else
    {
        return false;
    }
}