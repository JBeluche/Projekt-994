// Copyright by Creating Mountains


#include "Projekt994/Public/Player/Projekt994PlayerState.h"
#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"

// Sets default values
AZombieBase::AZombieBase()
{

}

// Called when the game starts or when spawned
void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	
}

uint8 AZombieBase::GetPointsForKill(FString BoneName)
{
	if(BoneName.Contains(FString("l")) || BoneName.Contains(FString("r")))
	{
		UE_LOG(LogTemp, Warning, TEXT("SHOT TO THE LIMB"));
		return 50;
	}
	else if(BoneName.Contains(FString("spine")) || BoneName.Contains(FString("pelvis")))
	{
		UE_LOG(LogTemp, Warning, TEXT("SHOT TO THE TORSO"));
		return 60;
	}
	else if (BoneName.Equals(FString("neck_01")))
	{
		UE_LOG(LogTemp, Warning, TEXT("SHOT TO THE NECK"));
		return 70;
	}
	else if (BoneName.Contains(FString("head")))
	{
		UE_LOG(LogTemp, Warning, TEXT("SHOT TO THE HEAD"));
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
