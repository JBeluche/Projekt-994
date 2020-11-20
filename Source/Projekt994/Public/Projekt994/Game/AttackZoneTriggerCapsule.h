// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerCapsule.h"
#include "AttackZoneTriggerCapsule.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AAttackZoneTriggerCapsule : public ATriggerCapsule
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:

	AAttackZoneTriggerCapsule();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
};
