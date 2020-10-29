// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994//Useables/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComopent");
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

TArray<int32> AWeaponBase::GetCurrentAmmo()
{
	return {CurrentMagazineAmmo, CurrentTotalAmmo};
}

void AWeaponBase::Fire()
{

}
	
void AWeaponBase::Reload()
{

}
