// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994//Useables/WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994GameState.h"
#include "Net/UnrealNetwork.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComopent");

    RootComponent = WeaponMesh;
    SetReplicates(true);
    WeaponMaxAmmo = 255;
    MagazineMaxAmmo = 30;
    WeaponName = "Default";

    CurrentTotalAmmo = WeaponMaxAmmo;
    CurrentMagazineAmmo = MagazineMaxAmmo;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
    WeaponMesh->HideBoneByName(FName("emptyCase_1"), EPhysBodyOp::PBO_None);
    WeaponMesh->HideBoneByName(FName("emptyCase_2"), EPhysBodyOp::PBO_None);
    WeaponMesh->HideBoneByName(FName("emptyCase_3"), EPhysBodyOp::PBO_None);
    WeaponMesh->HideBoneByName(FName("emptyCase_4"), EPhysBodyOp::PBO_None);
    WeaponMesh->HideBoneByName(FName("Magazine2"), EPhysBodyOp::PBO_None);
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(AWeaponBase, CurrentTotalAmmo, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(AWeaponBase, CurrentMagazineAmmo, COND_OwnerOnly);
}

TArray<int32> AWeaponBase::GetCurrentAmmo()
{
    return {CurrentMagazineAmmo, CurrentTotalAmmo};
}

void AWeaponBase::Server_Fire_Implementation(const TArray<FHitResult> &HitResults)
{
    if (CurrentMagazineAmmo > 0)
    {
        if (AProjekt994GameState *GS = GetWorld()->GetGameState<AProjekt994GameState>())
        {
            if (!GS->CheatIgnoreAmmo())
            {
                --CurrentMagazineAmmo;
            }
        }
        else
        {
            --CurrentMagazineAmmo;
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
        if (HitResults.Num() > 0)
        {
            Multi_Fire(HitResults[0]);
        }
        else
        {
            Multi_Fire(FHitResult());
        }
    }
}

bool AWeaponBase::Fire(AProjekt994Character *ShootingPlayer)
{

    if (CurrentMagazineAmmo > 0)
    {
        if (AProjekt994GameState *GS = GetWorld()->GetGameState<AProjekt994GameState>())
        {
            if (!GS->CheatIgnoreAmmo())
            {
                --CurrentMagazineAmmo;
            }
        }
        else
        {
            --CurrentMagazineAmmo;
        }

        //Play animation
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

        //Unlimited ammo cheat
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
        if (GetWorld()->IsServer())
        {
            if (HitResults.Num() > 0)
            {
                Multi_Fire(HitResults[0]);
            }
            else
            {
                Multi_Fire(FHitResult());
            }
        }
        else
        {
            Server_Fire(HitResults);
        }

        return true;
    }

    return false;
}

bool AWeaponBase::Multi_Fire_Validate(const FHitResult &HitResult)
{
    return true;
}

void AWeaponBase::Multi_Fire_Implementation(const FHitResult &HitResult)
{
     if (AProjekt994Character *Character = Cast<AProjekt994Character>(GetOwner()))
    {
        if (!Character->IsLocallyControlled() && FireAnimation)
        {
             if (UAnimInstance *AnimInstance = Character->GetMesh1P()->GetAnimInstance())
            {
                if (FPSArmsMontage)
                {
                    AnimInstance->Montage_Play(FPSArmsMontage);
                    if(Character->GetIsAiming())
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

TArray<FHitResult> AWeaponBase::PerformLineTrace(AProjekt994Character *ShootingPlayer)
{

    FVector Start = WeaponMesh->GetSocketLocation(FName("muzzleSocket"));
    FVector Rot = WeaponMesh->GetSocketRotation(FName("muzzleSocket")).Vector();

    FVector End = Start + Rot * 5000.0f;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.AddIgnoredActor(ShootingPlayer);

    FCollisionResponseParams CollisionResponse;

    GetWorld()->LineTraceMultiByChannel(OUT HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams, CollisionResponse);
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);

    return HitResults;
}

UAnimMontage *AWeaponBase::GetFPSAnimMontage()
{
    return FPSArmsMontage;
}

bool AWeaponBase::Server_Fire_Validate(const TArray<FHitResult> &HitResults)
{
    return true;
}

TArray<FHitResult> AWeaponBase::PerformLineTrace(FVector MuzzleLocation, FRotator MuzzleRotation)
{

    FVector End = MuzzleLocation + MuzzleRotation.Vector() * 5000.0f;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    if (GetOwner())
    {
        CollisionParams.AddIgnoredActor(GetOwner());
    }

    FCollisionResponseParams CollisionResponse;

    GetWorld()->LineTraceMultiByChannel(OUT HitResults, MuzzleLocation, End, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams, CollisionResponse);
    DrawDebugLine(GetWorld(), MuzzleLocation, End, FColor::Red, false, 2.0f, 0, 3.0f);

    return HitResults;
}

FWeaponDamage AWeaponBase::GetWeaponDamage()
{
    return WeaponDamage;
}

TEnumAsByte<EWeaponID> AWeaponBase::GetWeaponID()
{
    return WeaponID;
}

int8 AWeaponBase::Reload()
{
    if (CurrentTotalAmmo > 0 && CurrentMagazineAmmo != MagazineMaxAmmo)
    {
        bool bIsSlideLockReload = CurrentMagazineAmmo <= 0;
        if (APawn *Pawn = Cast<APawn>(GetOwner()))
        {
            if (Pawn->IsLocallyControlled())
            {
                if (CurrentMagazineAmmo <= 0 && ReloadEmptyAnimation)
                {
                    WeaponMesh->PlayAnimation(ReloadEmptyAnimation, false);
                }
                else if (ReloadAnimation)
                {
                    WeaponMesh->PlayAnimation(ReloadAnimation, false);
                }
            }
        }

        //Set ammo difference
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

        if (GetWorld()->IsServer())
        {
            Multi_Reload();
        }
        else
        {
            Server_Reload();
        }

        if (bIsSlideLockReload)
        {
            return 2;
        }
        else
        {
            return 1;
        }
    }

    return 0;
}

bool AWeaponBase::Server_Reload_Validate()
{
    return true;
}

void AWeaponBase::Server_Reload_Implementation()
{
    Reload();
    Multi_Reload();
}

bool AWeaponBase::Multi_Reload_Validate()
{
    return true;
}

void AWeaponBase::Multi_Reload_Implementation()
{
    if (AProjekt994Character *Character = Cast<AProjekt994Character>(GetOwner()))
    {
        if (!Character->IsLocallyControlled() && ReloadAnimation)
        {
            if (UAnimInstance *AnimInstance = Character->GetMesh()->GetAnimInstance())
            {
                if (ThirdPersonMontage)
                {
                    AnimInstance->Montage_Play(ThirdPersonMontage);
                    AnimInstance->Montage_JumpToSection(FName("Reload"), ThirdPersonMontage);
                }
            }
            WeaponMesh->PlayAnimation(ReloadAnimation, false);
        }
    }
}

int AWeaponBase::GetMagazineAmmo()
{
    return CurrentMagazineAmmo;
}

void AWeaponBase::StopFiring()
{
}
