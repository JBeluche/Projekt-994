// Copyright by Creating Mountains

#include "Projekt994/Public/Player/Projekt994PlayerState.h"
#include "Net/UnrealNetwork.h"


AProjekt994PlayerState::AProjekt994PlayerState()
{
    Points = 500;
}


void AProjekt994PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AProjekt994PlayerState, Points);
}

void AProjekt994PlayerState::OnRep_PointsChanged()
{
    NewPoints.Broadcast(Points);
}

void AProjekt994PlayerState::IncrementPoints(uint16 Value)
{
    if (HasAuthority())
    {
        Points += Value;
        OnRep_PointsChanged();
        UE_LOG(LogTemp, Warning, TEXT("Zombie hit: %d"), Points);
    }
}

bool AProjekt994PlayerState::DecrementPoints(uint16 Value)
{
    if (HasAuthority())
    {
        if (Points - Value < 0)
        {
            return false;
        }
        else
        {
            Points -= Value;
            OnRep_PointsChanged();
        }
        UE_LOG(LogTemp, Warning, TEXT("Zombie hit@!@ %d"), Points);
    }
    return true;
}

int32 AProjekt994PlayerState::GetPoints()
{
    return Points;
}

