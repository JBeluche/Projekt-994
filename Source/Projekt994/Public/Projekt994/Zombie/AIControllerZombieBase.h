// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerZombieBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AAIControllerZombieBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIControllerZombieBase();

	void SetIsInAttackZone();
	void ClearIsInAttackZone();

protected:
	virtual void BeginPlay() override;
	void Tick(float DeltaSeconds);


private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
	class AAttackZoneTriggerCapsule* GetClosestAttackZone();

	
};
