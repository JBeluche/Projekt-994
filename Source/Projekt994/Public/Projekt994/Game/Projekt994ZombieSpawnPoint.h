// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Projekt994ZombieSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AProjekt994ZombieSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	AProjekt994ZombieSpawnPoint();

protected:
	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		class ABarricade* LinkedBarricade;

		uint8 Zone;

		virtual void BeginPlay() override;

public:
	class ABarricade* GetLinkedBarricade();
	uint8 GetZone();
	void SetZone(uint8 ZoneNumber);

};
