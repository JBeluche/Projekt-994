// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum EHitLocation
{
	None UMETA(DisplayName = "None"),
	Head UMETA(DisplayName = "Head"),
	Torso UMETA(DisplayName = "Torso")
};

USTRUCT(BlueprintType)
struct FWeaponDamage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
		float BaseDamage = 20.0f;
	UPROPERTY(EditDefaultsOnly)
		float HeadMultiplier = 3.5f;
	UPROPERTY(EditDefaultsOnly)
		float TorsoMultiplier = 1.1f;		
		float GetDamage(EHitLocation HitLocation)
		{
			if (HitLocation == EHitLocation::Head)
			{
				return BaseDamage * HeadMultiplier;
			}
			else if (HitLocation == EHitLocation::Torso)
			{
				return BaseDamage * TorsoMultiplier;
			}
			else
			{
				return BaseDamage;
			}
			
		}
};

UCLASS()
class PROJEKT994_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		class UAnimMontage* FPSArmsFireMontage;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		class UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		int32 WeaponMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		int32 MagazineMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		FWeaponDamage WeaponDamage;

		int32 CurrentTotalAmmo;
		int32 CurrentMagazineAmmo;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FHitResult> PerformLineTrace(class AProjekt994Character* ShootingPlayer);
	TArray<FHitResult> PerformLineTrace(FVector MuzzleLocation, FRotator MuzzleRotation);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Fire(const TArray<FHitResult>& HitResults);
	bool Server_Fire_Validate(const TArray<FHitResult>& HitResults);
	virtual void Server_Fire_Implementation(const TArray<FHitResult>& HitResults);


public:	
	virtual TArray<FHitResult> Fire(class AProjekt994Character* ShootingPlayer);
	FWeaponDamage GetWeaponDamage();
	
	virtual void Reload();

	//First element is Magazine Ammo, second element is total ammo
	TArray<int32> GetCurrentAmmo();

	class UAnimMontage* GetFireAnimMontage();

};
