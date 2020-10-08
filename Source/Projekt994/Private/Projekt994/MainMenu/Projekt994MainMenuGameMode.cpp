// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994/MainMenu/Projekt994MainMenuGameMode.h"

#include "Projekt994/Public/Projekt994/MainMenu/Projekt994BeaconHostObject.h"
#include "OnlineBeaconHost.h"



AProjekt994MainMenuGameMode::AProjekt994MainMenuGameMode()
{
    HostObject = nullptr;
    Host = nullptr;
    bUseSeamlessTravel = false;

}


bool AProjekt994MainMenuGameMode::CreateHostBeacon()
{
    Host = GetWorld()->SpawnActor<AOnlineBeaconHost>(AOnlineBeaconHost::StaticClass());
    if (Host)
    {
        if (Host->InitHost())
        {
            Host->PauseBeaconRequests(false);

            HostObject = GetWorld()->SpawnActor<AProjekt994BeaconHostObject>(AProjekt994BeaconHostObject::StaticClass());
            if (HostObject)
            {
                Host->RegisterHost(HostObject);
                return true;
            }
        }
    }

    return false;
}

AProjekt994BeaconHostObject* AProjekt994MainMenuGameMode::GetBeaconHost()
{
    return HostObject;
}