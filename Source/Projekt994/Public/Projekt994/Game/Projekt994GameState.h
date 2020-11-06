// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Projekt994GameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AProjekt994GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AProjekt994GameState();


//Variables
protected:
	UPROPERTY(EditAnywhere)
		uint16 RoundNumber;//Set to rep
	uint8 ZombiesOnMap;//Set to rep
	uint16 TotalZombiesRemaining;//Set to rep

//Getters
public:
	uint16 GetRoundNumber();
	void IncrementRoundNumber();
	void SetTotalZombiesRemaining(const uint16& ZombieCount);
	uint16 GetTotalZombiesRemaining();
	void ZombieKilled();
	uint8 GetZombiesOnMap();
	void ZombieSpawned();
};
