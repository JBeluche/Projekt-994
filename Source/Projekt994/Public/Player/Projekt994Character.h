// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "Projekt994Character.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AProjekt994Character : public ACharacterBase
{
	GENERATED_BODY()

public:
	AProjekt994Character();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
