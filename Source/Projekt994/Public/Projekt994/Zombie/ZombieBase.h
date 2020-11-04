// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieBase.generated.h"

UCLASS()
class PROJEKT994_API AZombieBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieBase();

protected:
	int16 Health;

	UPROPERTY(ReplicatedUsing = OnRep_Die, EditAnywhere)
		bool bIsDead;
	UFUNCTION()
		void OnRep_Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DecrementHealth(int16 Damage);
	void Die();
	uint8 GetPointsForKill(FString BoneName);


public:
	void Hit(class AProjekt994Character* Player, FHitResult HitResult);

};
