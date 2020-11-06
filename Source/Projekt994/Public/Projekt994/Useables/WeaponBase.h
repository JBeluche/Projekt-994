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
	Neck UMETA(DisplayName = "Neck"),
	Chest UMETA(DisplayName = "Chest"),
	Abdomen UMETA(DisplayName = "Abdomen")
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
		float ChestMultiplier = 1.25f;	
	UPROPERTY(EditDefaultsOnly)
		float AbdomenMultiplier = 1.1f;			

		float GetDamage(EHitLocation HitLocation)
		{
			switch (HitLocation)
			{
				case Head: return BaseDamage * HeadMultiplier;
				case Chest: return BaseDamage * ChestMultiplier;
				case Abdomen: return BaseDamage * AbdomenMultiplier;
				default: return BaseDamage;
			}
		}
};
UENUM(BlueprintType)
enum EWeaponID
{
	Colt1911 UMETA(DisplayName = "Colt1911"),
	M1Carabine UMETA(DisplayName = "M1Carabine"),
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
		TEnumAsByte<EWeaponID> WeaponID;

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

	UPROPERTY(Replicated)
		int32 CurrentTotalAmmo;

	UPROPERTY(Replicated)
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
	virtual bool Fire(class AProjekt994Character* ShootingPlayer);
	FWeaponDamage GetWeaponDamage();
	
	virtual bool Reload();

	//First element is Magazine Ammo, second element is total ammo
	TArray<int32> GetCurrentAmmo();

	class UAnimMontage* GetFireAnimMontage();

	UFUNCTION(BlueprintCallable)
		TEnumAsByte<EWeaponID> GetWeaponID();

};
