// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994/Game/Projekt994ZombieSpawnPoint.h"
#include "Projekt994/Public/Projekt994//Useables/Barricade.h"


AProjekt994ZombieSpawnPoint::AProjekt994ZombieSpawnPoint()
{
    Zone = 0;
    bIsActive = false;
}


void AProjekt994ZombieSpawnPoint::BeginPlay()
{
    Super::BeginPlay();
}

class ABarricade* AProjekt994ZombieSpawnPoint::GetLinkedBarricade()
{
    return LinkedBarricade;
}

uint8 AProjekt994ZombieSpawnPoint::GetZone()
{
    return Zone;
}
void AProjekt994ZombieSpawnPoint::SetZone(uint8 ZoneNumber)
{
    Zone = ZoneNumber;
}

void AProjekt994ZombieSpawnPoint::Activate()
{
    bIsActive = true;
}

bool AProjekt994ZombieSpawnPoint::IsActive()
{
    return bIsActive;
}

