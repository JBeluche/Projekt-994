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
	uint16 RoundNumber;
	uint8 ZombiesOnMap;

//Getters
public:
	uint16 GetRoundNumber();
	void IncrementRoundNumber();

};
