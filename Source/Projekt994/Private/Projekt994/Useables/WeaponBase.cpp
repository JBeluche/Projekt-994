// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994//Useables/WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994GameState.h"
#include "Net/UnrealNetwork.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"
#include "TimerManager.h"

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

    bCanFire = true;
    DelayBetweenShots = 0.18;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();

    WeaponMesh->SetHiddenInGame(true);
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
    UE_LOG(LogTemp, Warning, TEXT("Server_Fire_Implementation wb"));

    if (CurrentMagazineAmmo > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ammo server fire is %d"), CurrentMagazineAmmo);

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
        bCanFire = false;

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

void AWeaponBase::ControlFireDelay()
{
    bCanFire = true;
}

void AWeaponBase::Fire()
{

    if (AProjekt994Character *Player = Cast<AProjekt994Character>(GetOwner()))

    {

        if (CurrentMagazineAmmo > 0 && bCanFire)
        {

            //Play animation
            if (UAnimInstance *AnimInstance = Player->GetMesh1P()->GetAnimInstance())
            {
                if (FPSArmsMontage)
                {
                    AnimInstance->Montage_Play(FPSArmsMontage);
                    if (Player->GetIsAiming())
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

            TArray<FHitResult> HitResults = PerformLineTrace(Player);

            if (HitResults.Num() > 0)
            {
                for (FHitResult &Result : HitResults)
                {
                    FString HitBone = Result.BoneName.ToString();
                    if (AActor *HitActor = Result.GetActor())
                    {
                        if (AZombieBase *Zombie = Cast<AZombieBase>(HitActor))
                        {
                            Zombie->Hit(Player, Result);
                        }
                    }
                }
            }
            if (!GetWorld()->IsServer())
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

            FTimerHandle FireRateHandle;
            GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this, &AWeaponBase::ControlFireDelay, DelayBetweenShots, false);
        }
    }
}

bool AWeaponBase::Multi_Fire_Validate(const FHitResult &HitResult)
{
    return true;
}

void AWeaponBase::Multi_Fire_Implementation(const FHitResult &HitResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Multi_Fire_Implementation wb"));

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
    //DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);

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
   // DrawDebugLine(GetWorld(), MuzzleLocation, End, FColor::Red, false, 2.0f, 0, 3.0f);

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

void AWeaponBase::Reload()
{
    if (CurrentTotalAmmo > 0 && CurrentMagazineAmmo != MagazineMaxAmmo)
    {
        if (AProjekt994Character *Player = Cast<AProjekt994Character>(GetOwner()))
        {
            bool bMagazineIsEmpty = CurrentMagazineAmmo <= 0;

            if (UAnimInstance *AnimInstance = Player->GetMesh1P()->GetAnimInstance())
            {
                if (FPSArmsMontage)
                {
                    bCanFire = false;
                    AnimInstance->Montage_Play(FPSArmsMontage);
                    float AnimationLength = 40.0f;
                    if (bMagazineIsEmpty)
                    {
                        AnimInstance->Montage_JumpToSection(FName("ReloadEmpty"), FPSArmsMontage);
                        AnimationLength = FPSArmsMontage->GetSectionLength(2);
                    }
                    else
                    {
                        AnimInstance->Montage_JumpToSection(FName("Reload"), FPSArmsMontage);
                        AnimationLength = FPSArmsMontage->GetSectionLength(3);
                    }

                    AnimationLength -= 0.05;
                    FTimerHandle ReloadHandle;

                    GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &AWeaponBase::ControlFireDelay, AnimationLength, false);
                }
            }

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
        }
    }
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

void AWeaponBase::ChangeFireMode()
{
}

void AWeaponBase::WeaponIsNowInHand(bool InHand)
{
    WeaponMesh->SetHiddenInGame(!InHand);
}
