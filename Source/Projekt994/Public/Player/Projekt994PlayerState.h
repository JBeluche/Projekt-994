// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Projekt994PlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointsChanged, int32, NewPoints);

UCLASS()
class PROJEKT994_API AProjekt994PlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AProjekt994PlayerState();

protected:

	UPROPERTY(BlueprintAssignable)
		FPointsChanged NewPoints;

	UPROPERTY(ReplicatedUsing = OnRep_PointsChanged, EditDefaultsOnly)//Set to replicate, move to plauer state when created
		int32 Points;
	UFUNCTION()
		void OnRep_PointsChanged();

public:
	void IncrementPoints(uint16 Value);
	bool DecrementPoints(uint16 Value);

	UFUNCTION(BlueprintCallable)
		int32 GetPoints();
};
