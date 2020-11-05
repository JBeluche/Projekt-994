// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994/Game/Projekt994GameState.h"

AProjekt994GameState::AProjekt994GameState()
{
    RoundNumber = 1;
    ZombiesOnMap = 0;
    TotalZombiesRemaining = 5;
    
}

uint16 AProjekt994GameState::GetRoundNumber()
{
    return RoundNumber;
}
void AProjekt994GameState::IncrementRoundNumber()
{
    ++RoundNumber;
}

uint16 AProjekt994GameState::GetTotalZombiesRemaining()
{
    return TotalZombiesRemaining;
}

void AProjekt994GameState::SetTotalZombiesRemaining(const uint16& ZombieCount)
{
    TotalZombiesRemaining = ZombieCount;
}

void AProjekt994GameState::ZombieKilled()
{
    --TotalZombiesRemaining;
    --ZombiesOnMap;
}
uint8 AProjekt994GameState::GetZombiesOnMap()
{
    UE_LOG(LogTemp, Warning, TEXT("Zombies on map: %d"), ZombiesOnMap);

    return ZombiesOnMap;
}

void AProjekt994GameState::ZombieSpawned()
{
    ++ZombiesOnMap;
}
