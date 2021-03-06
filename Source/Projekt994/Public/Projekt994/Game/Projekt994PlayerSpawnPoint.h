// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Projekt994PlayerSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AProjekt994PlayerSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public: 
	AProjekt994PlayerSpawnPoint();

protected:
	bool bIsUsed;

public:
	bool IsUsed();
	void SetUsed(bool Used);
};
