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
            Multi_Fire(HitResults[0]);
        }
    }
}

void AWeaponSemiAutomatic::Multi_Fire_Implementation(const FHitResult &HitResult)
{
    if (AProjekt994Character *Character = Cast<AProjekt994Character>(GetOwner()))
    {
        if (!Character->IsLocallyControlled() && FireAnimation)
        {
             if (UAnimInstance *AnimInstance = Character->GetMesh()->GetAnimInstance())
            {
                if (ThirdPersonMontage)
                {
                    AnimInstance->Montage_Play(ThirdPersonMontage);
                    if(Character->GetIsAiming())
                    {
                        AnimInstance->Montage_JumpToSection(FName("FireADS"), ThirdPersonMontage);
                    }
                    else
                    {
                        AnimInstance->Montage_JumpToSection(FName("FireHip"), ThirdPersonMontage);
                    }
                    
                   
                }
            }
            WeaponMesh->PlayAnimation(FireAnimation, false);
        }
    }
}

bool AWeaponSemiAutomatic::Reload()
{
    return Super::Reload();
}