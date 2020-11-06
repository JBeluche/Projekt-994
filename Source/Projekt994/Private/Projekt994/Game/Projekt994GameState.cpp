// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994/Game/Projekt994GameState.h"
#include "Net/UnrealNetwork.h"

AProjekt994GameState::AProjekt994GameState()
{
    RoundNumber = 1;
    ZombiesOnMap = 0;
    TotalZombiesRemaining = 5;
    
}

void AProjekt994GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AProjekt994GameState, bIgnoreAmmo);
}

void AProjekt994GameState::sf_use_ignoreAmmo(bool IgnoreAmmo)
{
    if(HasAuthority())
    {
        bIgnoreAmmo = IgnoreAmmo;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("You are not the server bro..."));
    }
    
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
    return ZombiesOnMap;
}

void AProjekt994GameState::ZombieSpawned()
{
    ++ZombiesOnMap;
}
