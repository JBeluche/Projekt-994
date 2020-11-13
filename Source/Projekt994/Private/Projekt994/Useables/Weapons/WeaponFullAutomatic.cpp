// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994//Useables/Weapons/WeaponFullAutomatic.h"

//Game Include
#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"

//Engine Includes
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

AWeaponFullAutomatic::AWeaponFullAutomatic()
{
    WeaponMaxAmmo = 240;
    MagazineMaxAmmo = 30;
    WeaponName = "Default Name";
    bIsInFullAuto = true;
    bIsSelectFire = true;
    FireRate = 500;

}

void AWeaponFullAutomatic::BeginPlay()
{
    Super::BeginPlay();
    CurrentTotalAmmo = WeaponMaxAmmo;
    CurrentMagazineAmmo = MagazineMaxAmmo;

    FireRate = 60 / FireRate;
}

void AWeaponFullAutomatic::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(AWeaponFullAutomatic, bIsFiring, COND_SkipOwner);
}

bool AWeaponFullAutomatic::Server_StartFullAutoFire_Validate(bool IsFiring)
{
    return true;
}

void AWeaponFullAutomatic::Server_StartFullAutoFire_Implementation(bool IsFiring)
{
        UE_LOG(LogTemp, Warning, TEXT("Server_StartFullAutoFire_Implementation"));

    bIsFiring = IsFiring;
    OnRep_StartFullAutoFire();
}

void AWeaponFullAutomatic::OnRep_StartFullAutoFire()
{
        UE_LOG(LogTemp, Warning, TEXT("OnRep_StartFullAutoFire"));

    if (bIsFiring)
    {
        GetWorld()->GetTimerManager().SetTimer(WeaponFireHandle, this, &AWeaponFullAutomatic::OnClientFire, FireRate, true);
        PlayWeaponEffects();
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(WeaponFireHandle);
    }
}

void AWeaponFullAutomatic::PlayWeaponEffects()
{
        UE_LOG(LogTemp, Warning, TEXT("PlayWeaponEffects"));

    if (AProjekt994Character *Character = Cast<AProjekt994Character>(GetOwner()))
    {
        if (!Character->IsLocallyControlled() && FireAnimation)
        {
            if (UAnimInstance *AnimInstance = Character->GetMesh1P()->GetAnimInstance())
            {
                if (FPSArmsMontage)
                {
                    AnimInstance->Montage_Play(FPSArmsMontage);
                    if (Character->GetIsAiming())
                    {
                        AnimInstance->Montage_JumpToSection(FName("FireADS"), FPSArmsMontage);
                    }
                    else
                    {
                        AnimInstance->Montage_JumpToSection(FName("FireHip"), FPSArmsMontage);
                    }
                }
            }
            WeaponMesh->PlayAnimation(FireAnimation, false);
        }
    }
}

void AWeaponFullAutomatic::OnClientFire()
{
        UE_LOG(LogTemp, Warning, TEXT("OnClientFire"));

    if (CurrentMagazineAmmo > 0)
    {

        if (AProjekt994Character *ShootingPlayer = Cast<AProjekt994Character>(GetOwner()))
        {
            --CurrentMagazineAmmo;

            if (UAnimInstance *AnimInstance = ShootingPlayer->GetMesh1P()->GetAnimInstance())
            {
                if (FPSArmsMontage)
                {
                    AnimInstance->Montage_Play(FPSArmsMontage);
                    if (ShootingPlayer->GetIsAiming())
                    {

                        AnimInstance->Montage_JumpToSection(FName("FireADS"), FPSArmsMontage);
                    }
                    else
                    {

                        AnimInstance->Montage_JumpToSection(FName("FireHip"), FPSArmsMontage);
                    }
                }
            }

            if (CurrentMagazineAmmo <= 0 && FireEmptyAnimation)
            {
                WeaponMesh->PlayAnimation(FireEmptyAnimation, false);
            }
            else if (FireAnimation)
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
        }
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(WeaponFireHandle);
        bIsFiring = false;
        if (!GetWorld()->IsServer())
        {
            Server_StartFullAutoFire(bIsFiring);
        }
    }
}

void AWeaponFullAutomatic::Server_Fire_Implementation(const TArray<FHitResult> &HitResults)
{
        UE_LOG(LogTemp, Warning, TEXT("Server_Fire_Implementation"));

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
    }
}

void AWeaponFullAutomatic::Fire()
{
        UE_LOG(LogTemp, Warning, TEXT("Fire"));

    if (!bIsFiring && bIsInFullAuto)
    {
        bIsFiring = true;
        OnClientFire();

        GetWorld()->GetTimerManager().SetTimer(WeaponFireHandle, this, &AWeaponFullAutomatic::OnClientFire, FireRate, true);

        if (!GetWorld()->IsServer())
        {
            Server_StartFullAutoFire(bIsFiring);
        }
    }
    else
    {
        Super::Fire();
    }
}

void AWeaponFullAutomatic::StopFiring()
{
        UE_LOG(LogTemp, Warning, TEXT("StopFiring"));

    bIsFiring = false;
    GetWorld()->GetTimerManager().ClearTimer(WeaponFireHandle);
    if (!GetWorld()->IsServer())
    {
        Server_StartFullAutoFire(bIsFiring);
    }
}

void AWeaponFullAutomatic::ChangeFireMode()
{
    if (bIsSelectFire)
    {
        bIsInFullAuto = !bIsInFullAuto;
    }
}