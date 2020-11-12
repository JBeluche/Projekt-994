// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Projekt994/Useables/WeaponBase.h"
#include "WeaponFullAutomatic.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AWeaponFullAutomatic : public AWeaponBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)//Replicated for using effects on other clients 
		bool bIsFiring;

		UFUNCTION()
		void OnRep_StartFullAutoFire();

		FTimerHandle WeaponFireHandle;

		void PlayWeaponEffects();
	
public:
AWeaponFullAutomatic();


protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_StartFullAutoFire(bool IsFiring);
	bool Server_StartFullAutoFire_Validate(bool IsFiring);
	void Server_StartFullAutoFire_Implementation(bool IsFiring);

	virtual void Server_Fire_Implementation(const TArray<FHitResult>& HitResults) override;



public:
	virtual bool  Fire(class AProjekt994Character* ShootingPlayer) override;
	virtual void StopFiring() override;
};
