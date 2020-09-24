// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Projekt994MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AProjekt994MainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjekt994MainMenuGameMode();
	
protected:
	UFUNCTION(BlueprintCallable)
		bool CreateHostBeacon();

		class AProjekt994BeaconHostObject* HostObject;

		UFUNCTION(BlueprintCallable)
			class AProjekt994BeaconHostObject* GetBeaconHost();



};
