// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994/MainMenu/Projekt994BeaconHostObject.h"
#include "Projekt994/Public/Projekt994/MainMenu/Projekt994BeaconClient.h"



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

void AProjekt994BeaconHostObject::DisconnectAllClients()
{
        UE_LOG(LogTemp, Warning, TEXT("Disconecting all clients"));

        for(AOnlineBeaconClient* Client : ClientActors)
        {
            if(Client){
                DisconnectClient(Client);
            }
        }

        Unregister();

}