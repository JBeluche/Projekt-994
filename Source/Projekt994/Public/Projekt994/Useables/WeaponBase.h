// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

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
		class UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		int32 WeaponMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		int32 MagazineMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		int32 BaseDamage;

		int32 CurrentTotalAmmo;
		int32 CurrentMagazineAmmo;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FHitResult> PerformLineTrace(class AProjekt994Character* ShootingPlayer);

public:	
	virtual TArray<FHitResult> Fire(class AProjekt994Character* ShootingPlayer);
	
	virtual void Reload();

	//First element is Magazine Ammo, second element is total ammo
	TArray<int32> GetCurrentAmmo();

};
