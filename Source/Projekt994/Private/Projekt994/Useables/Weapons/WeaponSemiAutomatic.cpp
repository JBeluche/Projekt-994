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
}

void AWeaponSemiAutomatic::BeginPlay()
{
    Super::BeginPlay();
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
        if (GetWorld()->IsServer())
        {
            Multi_Fire(HitResults[0]);
        }
        else
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
        Multi_Fire(HitResults[0]);
    }
}

void AWeaponSemiAutomatic::Multi_Fire_Implementation(const FHitResult &HitResult)
{
    if (APawn * Pawn = Cast<APawn>(GetOwner()))
    {
        if (!Pawn->IsLocallyControlled() && FireAnimation)
        {
            UE_LOG(LogTemp, Error, TEXT("Made it to the multi fire implementaion"));
            WeaponMesh->PlayAnimation(FireAnimation, false);
        }
    }
}

bool AWeaponSemiAutomatic::Reload()
{
    return Super::Reload();
}