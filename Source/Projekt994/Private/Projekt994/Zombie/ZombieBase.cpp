// Copyright by Creating Mountains


#include "Projekt994/Public/Player/Projekt994PlayerState.h"
#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"

#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AZombieBase::AZombieBase()
{
	Health = 150;
	bIsDead = false;
	CleanupDelay = 5.0f;
}

// Called when the game starts or when spawned
void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombieBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZombieBase, bIsDead)
}


uint8 AZombieBase::GetPointsForKill(FString BoneName)
{
	if(BoneName.Contains(FString("l")) || BoneName.Contains(FString("r")))
	{
		DecrementHealth(30);
		return 50;
	}
	else if(BoneName.Contains(FString("spine")) || BoneName.Contains(FString("pelvis")))
	{
		DecrementHealth(50);
		return 60;
	}
	else if (BoneName.Equals(FString("neck_01")))
	{
		DecrementHealth(70);
		return 70;
	}
	else if (BoneName.Contains(FString("head")))
	{
		DecrementHealth(90);
		return 100;
	}

	return 0;

}


void AZombieBase::Hit(class AProjekt994Character* Player, FHitResult HitResult)
{
	if(Player)
	{
		if(AProjekt994PlayerState* PState = Player->GetPlayerState<AProjekt994PlayerState>())
		{
			FString BoneName = HitResult.BoneName.ToString();
			if(BoneName == FString("None"))
			{
				return;
			}
			else
			{
				PState->IncrementPoints(GetPointsForKill(BoneName));
			}
		}
	}
}

void AZombieBase::DecrementHealth(int16 Damage)
{
	if(HasAuthority())
	{
		Health -= Damage;
		if(Health <= 0)
		{
			Die();
		}
	}

}

void AZombieBase::Die()
{
	if(HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Zombie died"));
		bIsDead = true;
		OnRep_Die();
	}

}

void AZombieBase::OnRep_Die()
{
		UE_LOG(LogTemp, Warning, TEXT("Onrep died"));
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
		if(HasAuthority())
		{
			if(AController* AIController = GetController<AController>())
			{
				Controller->Destroy();
			}
			FTimerHandle TempHandle;
			GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &AZombieBase::OnCleanup, CleanupDelay, false);
		}
	
}

void AZombieBase::OnCleanup()
{
	Destroy();
}

