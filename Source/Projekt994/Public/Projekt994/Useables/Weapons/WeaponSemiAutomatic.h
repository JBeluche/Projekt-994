// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Projekt994/Useables/WeaponBase.h"
#include "WeaponSemiAutomatic.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AWeaponSemiAutomatic : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeaponSemiAutomatic();

protected:
	virtual void Server_Fire_Implementation(const TArray<FHitResult>& HitResults) override;
	virtual void BeginPlay() override;

public:
	virtual bool  Fire(class AProjekt994Character* ShootingPlayer) override;
	
	virtual bool Reload() override;


};
