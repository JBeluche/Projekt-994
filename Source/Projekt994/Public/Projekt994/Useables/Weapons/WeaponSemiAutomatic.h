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
	virtual void BeginPlay() override;

	
};
