// Copyright by Creating Mountains


#include "Projekt994/Public/Player/Projekt994PlayerState.h"
#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994GameModeBase.h"
#include "Projekt994/Public/Projekt994//Useables/WeaponBase.h"
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

uint8 AZombieBase::GetHitPart(FString BoneName)
{
	if(BoneName.Contains(FString("l")) || BoneName.Contains(FString("r")))
	{
		return 1;
	}
	else if(BoneName.Contains(FString("spine")) || BoneName.Contains(FString("pelvis")))
	{
		return 2;
	}
	else if (BoneName.Equals(FString("neck_01")))
	{
		return 3;
	}
	else if (BoneName.Contains(FString("head")))
	{
		return 4;
	}
	return 0;

}


uint8 AZombieBase::GetPointsForHit(uint8 HitPart, float Damage)
{
	if(Health - Damage <= 0)
	{
		switch(HitPart)
		{
			case 1: {DecrementHealth(Damage);  return 50;}
			case 2: {DecrementHealth(Damage); return 60;}
			case 3: {DecrementHealth(Damage); return 70;}
			case 4: {DecrementHealth(Damage); return 100;}
			default: return 0;
		}
	}
	else
	{
		DecrementHealth(Damage);
		return 10;
	}


	return 0;

}


void AZombieBase::Hit(class AProjekt994Character* Player, FHitResult HitResult)
{
	if(Player && !bIsDead)
	{
		if(AProjekt994PlayerState* PState = Player->GetPlayerState<AProjekt994PlayerState>())
		{
			FString BoneName = HitResult.BoneName.ToString();
			if(BoneName == FString("None"))
			{
				return;
			}
			if(uint8 HitPart = GetHitPart(BoneName))
			{
				if(AWeaponBase* PlayerWeapon = Player->GetCurrentWeapon())
				{
				EHitLocation HitLocation = EHitLocation::None;
				switch (HitPart)
				{
					case 2: HitLocation = EHitLocation::Torso;
					case 3: HitLocation = EHitLocation::Head;
					case 4: HitLocation = EHitLocation::Head;
				}

				float WeaponDamage = PlayerWeapon->GetWeaponDamage().GetDamage(HitLocation);
				UE_LOG(LogTemp, Warning, TEXT("Weapon Damage: %f"), WeaponDamage);

				if(uint8 PointsForHit = GetPointsForHit(HitPart, WeaponDamage))
				{
					PState->IncrementPoints(PointsForHit);
				}
				}
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
		//get game mode and do zombie decrementation
		if(AProjekt994GameModeBase* GM = GetWorld()->GetAuthGameMode<AProjekt994GameModeBase>())
		{
			GM->ZombieKilled();
		}
		bIsDead = true;
		OnRep_Die();
	}

}

void AZombieBase::OnRep_Die()
{
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



