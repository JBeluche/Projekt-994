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

	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
};
