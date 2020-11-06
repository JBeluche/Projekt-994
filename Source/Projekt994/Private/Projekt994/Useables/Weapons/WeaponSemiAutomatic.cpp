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

TArray<FHitResult>  AWeaponSemiAutomatic::Fire(AProjekt994Character* ShootingPlayer)
{
    if(FireAnimation)
    {
        WeaponMesh->PlayAnimation(FireAnimation, false);
    }

    if(GetWorld()->IsServer())
    {

    TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);

        if(HitResults.Num() > 0)
        {
            for (FHitResult& Result : HitResults)
            {
                FString HitBone = Result.BoneName.ToString();
                if (AActor* HitActor = Result.GetActor())
                {
                    if (AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
                        {
                            Zombie->Hit(ShootingPlayer, Result);
                        }
                }
            }
        }
    }
    else
    {
        
    TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);

        if(HitResults.Num() > 0)
        {
            for (FHitResult& Result : HitResults)
            {
                if (AActor* HitActor = Result.GetActor())
                {
                    if (AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
                        {
                            Zombie->Hit(ShootingPlayer, Result);
                        }
                }
            }
        }

        Server_Fire(HitResults);

    }

    return TArray<FHitResult>();
}

void AWeaponSemiAutomatic::Reload()
{

}

void AWeaponSemiAutomatic::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
{
     if(FireAnimation)
    {
        WeaponMesh->PlayAnimation(FireAnimation, false);
    }

    if(HitResults.Num() > 0)
    {

        for (FHitResult Result : HitResults)
        {
            if (AActor* HitActor = Result.GetActor())
            {
                  if (AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
                    {
                        if(AProjekt994Character* Player = Cast<AProjekt994Character>(GetOwner()))
                        {
                            Zombie->Hit(Player, Result);
                        }
                    }
            }
        }
      
    }
}

