// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994//Useables/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Projekt994/Public/Player/Projekt994Character.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComopent");
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
