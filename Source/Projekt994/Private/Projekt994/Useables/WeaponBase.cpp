// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994//Useables/WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994GameState.h"
#include "Net/UnrealNetwork.h"

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
    }

    return true;
}

bool AWeaponBase::Multi_Fire_Validate(const FHitResult &HitResult)
{
    return true;
}

void AWeaponBase::Multi_Fire_Implementation(const FHitResult &HitResult)
{
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

UAnimMontage *AWeaponBase::GetFireAnimMontage()
{
    return FPSArmsFireMontage;
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

bool AWeaponBase::Reload()
{
    if (CurrentTotalAmmo > 0 && CurrentMagazineAmmo != MagazineMaxAmmo)
    {
        if (APawn *Pawn = Cast<APawn>(GetOwner()))
        {
            if (Pawn->IsLocallyControlled() && ReloadAnimation)
            {
                WeaponMesh->PlayAnimation(ReloadAnimation, false);
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

        return true;
    }
    else
    {
        return false;
    }
}

bool AWeaponBase::Server_Reload_Validate()
{
    return true;
}

void AWeaponBase::Server_Reload_Implementation()
{
    if (APawn *Pawn = Cast<APawn>(GetOwner()))
        {
            if (!Pawn->IsLocallyControlled() && ReloadAnimation)
            {
                WeaponMesh->PlayAnimation(ReloadAnimation, false);
            }
        }
}

bool AWeaponBase::Multi_Reload_Validate()
{
    return true;
}

void AWeaponBase::Multi_Reload_Implementation()
{
    if (ReloadAnimation)
    {
        WeaponMesh->PlayAnimation(ReloadAnimation, false);
    }
}
