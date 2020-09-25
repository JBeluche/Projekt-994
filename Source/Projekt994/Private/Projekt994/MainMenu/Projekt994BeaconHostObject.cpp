// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994/MainMenu/Projekt994BeaconHostObject.h"
#include "Projekt994/Public/Projekt994/MainMenu/Projekt994BeaconClient.h"
#include "Projekt994/Public/Projekt994/MainMenu/Projekt994MainMenuGameMode.h"

#include "OnlineBeaconHost.h"

AProjekt994BeaconHostObject::AProjekt994BeaconHostObject()
{
    ClientBeaconActorClass = AProjekt994BeaconClient::StaticClass();
    BeaconTypeName = ClientBeaconActorClass->GetName();
}

void AProjekt994BeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
    Super::OnClientConnected(NewClientActor, ClientConnection);

    if (NewClientActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Connect to client VALID"))
    }
    else 
    {
        UE_LOG(LogTemp, Warning, TEXT("Connect to client INVALID"))

    }
}

void AProjekt994BeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor)
{
    Super::NotifyClientDisconnected(LeavingClientActor);

        UE_LOG(LogTemp, Warning, TEXT("Client has disconnected"));

}

void AProjekt994BeaconHostObject::ShutdownServer()
{
    //Unregister Server From Database Via Web API
    DisconnectAllClients();

    if (AOnlineBeaconHost* Host = Cast<AOnlineBeaconHost>(GetOwner()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Host has been destroyed"));
        Host->UnregisterHost(BeaconTypeName);
        Host->DestroyBeacon();
    }
}

void AProjekt994BeaconHostObject::DisconnectAllClients()
{
        UE_LOG(LogTemp, Warning, TEXT("Disconecting all clients"));

        for(AOnlineBeaconClient* Client : ClientActors)
        {
            if(Client){
                DisconnectClient(Client);
            }
        }
}


void AProjekt994BeaconHostObject::DisconnectClient(AOnlineBeaconClient* ClientActor)
{

    AOnlineBeaconHost* BeaconHost = Cast<AOnlineBeaconHost>(GetOwner());
    if(BeaconHost)
    {
        if (AProjekt994BeaconClient* Client = Cast<AProjekt994BeaconClient>(ClientActor))
        {
        UE_LOG(LogTemp, Warning, TEXT("Disconecting client: %s"), *ClientActor->GetName());
            Client->Client_OnDisconnected();
        }
        BeaconHost->DisconnectClient(ClientActor);
    }
}