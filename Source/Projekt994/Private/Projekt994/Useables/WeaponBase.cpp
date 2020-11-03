// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994//Useables/WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Projekt994/Public/Player/Projekt994Character.h"

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
	BaseDamage = 100;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<int32> AWeaponBase::GetCurrentAmmo()
{
	return {CurrentMagazineAmmo, CurrentTotalAmmo};
}

TArray<FHitResult>  AWeaponBase::Fire(AProjekt994Character* ShootingPlayer)
{
	return TArray<FHitResult>();
}
	
void AWeaponBase::Reload()
{

}

TArray<FHitResult> AWeaponBase::PerformLineTrace(AProjekt994Character* ShootingPlayer)
{
	    UE_LOG(LogTemp, Warning, TEXT("Shooting 1991"));

    FVector Start = WeaponMesh->GetSocketLocation(FName("muzzleSocket"));
    FVector Rot =  WeaponMesh->GetSocketRotation(FName("muzzleSocket")).Vector();

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

UAnimMontage* AWeaponBase::GetFireAnimMontage()
{
	return FPSArmsFireMontage;
}


