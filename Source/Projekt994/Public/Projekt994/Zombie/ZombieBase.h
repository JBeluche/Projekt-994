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

	UPROPERTY(EditDefaultsOnly)
		float CleanupDelay;

	UPROPERTY(ReplicatedUsing = OnRep_Die, EditAnywhere)
		bool bIsDead;
	UFUNCTION()
		void OnRep_Die();
	void OnCleanup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//0 = None, 1 = Limb, 2 = chest, 3 = abdomen, 4 = neck, 5 = head
	uint8 GetHitPart(FString BoneName);

	void DecrementHealth(int16 Damage);
	void Die();
	uint8 GetPointsForHit(uint8 HitPart, float Damage);

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
	TSubclassOf<class AAttackZoneTriggerCapsule> AttackZone;

public:
	void Hit(class AProjekt994Character* Player, FHitResult HitResult);

};
