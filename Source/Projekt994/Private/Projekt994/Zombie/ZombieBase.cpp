// Copyright by Creating Mountains


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

void AZombieBase::Hit(class AProjekt994Character* Player)
{
	if(HasAuthority() && Player)
	{
		Player->IncrementPoints(100);
	}
}
