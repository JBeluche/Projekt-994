// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994//Useables/Weapon1911.h"
#include "Components/SkeletalMeshComponent.h"
#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"
#include "Engine/World.h"

AWeapon1911::AWeapon1911()
{
    WeaponMaxAmmo = 98;
    MagazineMaxAmmo = 7;
    WeaponName = "1911";

    CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
	BaseDamage = 100;

}

TArray<FHitResult>  AWeapon1911::Fire(AProjekt994Character* ShootingPlayer)
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
                if (AActor* HitActor = Result.GetActor())
                {
                    if (AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
                        {
                            Zombie->Hit(ShootingPlayer);
                        }
                    UE_LOG(LogTemp, Warning, TEXT("Hit Actor!! %s"), *HitActor->GetName());
                }
            }
        }
    }
    else
    {
        Server_Fire(WeaponMesh->GetSocketLocation(FName("muzzleSocket")), WeaponMesh->GetSocketRotation(FName("muzzleSocket")));

    }

    return TArray<FHitResult>();
}

void AWeapon1911::Reload()
{

}

void AWeapon1911::Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
    UE_LOG(LogTemp, Warning, TEXT("server fires function 1911"));
     if(FireAnimation)
    {
        WeaponMesh->PlayAnimation(FireAnimation, false);
    }

    TArray<FHitResult> HitResults = PerformLineTrace(MuzzleLocation, MuzzleRotation);

    if(HitResults.Num() > 0)
    {

        for (FHitResult& Result : HitResults)
        {
            if (AActor* HitActor = Result.GetActor())
            {
                  if (AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
                    {
                        if(AProjekt994Character* Player = Cast<AProjekt994Character>(GetOwner()))
                        {
                            Zombie->Hit(Player);
                        }
                    }
                UE_LOG(LogTemp, Warning, TEXT("Hit Actor!! %s"), *HitActor->GetName());
            }
        }
      
    }
}

