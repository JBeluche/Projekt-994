// Copyright by Creating Mountains

#pragma once

#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"
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
	bool bHasLoadedSpawnPoints;
	TArray<class AProjekt994PlayerSpawnPoint*> PlayerSpawnPoints;
	TArray<class AProjekt994ZombieSpawnPoint*> ZombieSpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Projekt994Settings")
	TSubclassOf<class AZombieBase> ZombieClass;

	FTimerHandle TZombieSpawnHandle;	
 
//Spawn zombies
protected:
	void SpawnZombie();

//Functions
protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void SetSpawnPoints();

	
};
