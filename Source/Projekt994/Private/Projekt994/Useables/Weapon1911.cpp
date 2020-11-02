// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994//Useables/Weapon1911.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
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
    UE_LOG(LogTemp, Warning, TEXT("Shooting 1991"));

    FVector Start = WeaponMesh->GetSocketLocation(FName("muzzleSocket"));
    FVector Rot =  WeaponMesh->GetSocketRotation(FName("muzzleSocket")).Vector();

    FVector End = Start + Rot * 2000.0f;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.AddIgnoredActor(ShootingPlayer);

    FCollisionResponseParams CollisionResponse;

    if(GetWorld()->LineTraceMultiByChannel(OUT HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams, CollisionResponse))
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

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);

    return HitResults;
}

void AWeapon1911::Reload()
{

}
