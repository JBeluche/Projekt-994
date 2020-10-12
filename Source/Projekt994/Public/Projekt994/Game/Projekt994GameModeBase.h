// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Projekt994GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AProjekt994GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public: 
	AProjekt994GameModeBase();

//Variables
protected:
	TArray<class AProjekt994PlayerSpawnPoint*> PlayerSpawnPoints;
	bool bHasLoadedSpawnPoints;


	UPROPERTY(EditAnywhere, Category = "Projekt994Settings")
		TSubclassOf<class AZombieBase> ZombieClass;
 

//Functions
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void SetSpawnPoints();
};
