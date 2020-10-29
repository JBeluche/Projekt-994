// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Projekt994/Useables/WeaponBase.h"
#include "Weapon1911.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AWeapon1911 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeapon1911();

	virtual void Fire() override;
	
	virtual void Reload() override;
};
