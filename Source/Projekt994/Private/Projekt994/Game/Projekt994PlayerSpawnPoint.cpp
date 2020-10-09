// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994/Game/Projekt994PlayerSpawnPoint.h"

AProjekt994PlayerSpawnPoint::AProjekt994PlayerSpawnPoint()
{
    bIsUsed = false;
}

bool AProjekt994PlayerSpawnPoint::IsUsed()
{
    return bIsUsed;
}

void AProjekt994PlayerSpawnPoint::SetUsed(bool Used)
{
    bIsUsed = Used;
}

