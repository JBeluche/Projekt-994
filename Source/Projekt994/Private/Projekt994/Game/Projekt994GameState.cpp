// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994/Game/Projekt994GameState.h"

AProjekt994GameState::AProjekt994GameState()
{
    RoundNumber = 1;
    ZombiesOnMap = 0;
    
}

uint16 AProjekt994GameState::GetRoundNumber()
{
    return RoundNumber;
}
void AProjekt994GameState::IncrementRoundNumber()
{
    ++RoundNumber;
}



