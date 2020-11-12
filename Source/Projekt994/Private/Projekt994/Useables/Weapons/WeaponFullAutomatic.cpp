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

}

void AWeaponFullAutomatic::BeginPlay()
{
    Super::BeginPlay();
    CurrentTotalAmmo = WeaponMaxAmmo;
    CurrentMagazineAmmo = MagazineMaxAmmo;


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
    bIsFiring = IsFiring;
    OnRep_StartFullAutoFire();
}

void AWeaponFullAutomatic::OnRep_StartFullAutoFire()
{
    if(bIsFiring)
    {
        GetWorld()->GetTimerManager().SetTimer(WeaponFireHandle, this, &AWeaponFullAutomatic::PlayWeaponEffects, .5f, true);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(WeaponFireHandle);
    }
    
}

void AWeaponFullAutomatic::PlayWeaponEffects()
{   
        UE_LOG(LogTemp, Warning, TEXT("Onrep playing weapon effects for full auto class"));

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


bool AWeaponFullAutomatic::Fire(AProjekt994Character *ShootingPlayer)
{

    if(!bIsFiring)
    {
        bIsFiring = true;
        Server_StartFullAutoFire(bIsFiring);
    }

    return true;
   
}

void AWeaponFullAutomatic::Server_Fire_Implementation(const TArray<FHitResult> &HitResults)
{
    
}

void AWeaponFullAutomatic::StopFiring()
{
        bIsFiring = false;
        Server_StartFullAutoFire(bIsFiring);
}



