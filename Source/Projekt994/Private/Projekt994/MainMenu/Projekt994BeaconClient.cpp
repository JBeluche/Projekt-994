// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994/MainMenu/Projekt994BeaconClient.h"

AProjekt994BeaconClient::AProjekt994BeaconClient()
{
    PlayerIndex = 0;
}


bool AProjekt994BeaconClient::ConnectToServer(const FString& Address)
{
    FURL Destination = FURL(nullptr, *Address, ETravelType::TRAVEL_Absolute);
    Destination.Port= 7787;
    
    UE_LOG(LogTemp, Warning, TEXT("Connect to server"));

    return InitClient(Destination);
}

void AProjekt994BeaconClient::OnFailure()
{
    UE_LOG(LogTemp, Warning, TEXT("CLIENT FAILED TO CONNECT TO HOST BEACON"));
    FOnConnected.Broadcast(false);
}

void AProjekt994BeaconClient::OnConnected()
{
    UE_LOG(LogTemp, Warning, TEXT("CLIENT SUCCEDED TO CONNECT TO HOST BEACON"));
    FOnConnected.Broadcast(true);
}

void AProjekt994BeaconClient::LeaveLobby()
{
    DestroyBeacon();
}

void AProjekt994BeaconClient::Client_OnDisconnected_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Disconnected"));
    FOnDisconnected.Broadcast();
}

void AProjekt994BeaconClient::Client_OnLobbyUpdated_Implementation(FProjekt994LobbyInfo LobbyInfo)
{
    FOnLobbyUpdated.Broadcast(LobbyInfo);

}

void AProjekt994BeaconClient::SetPlayerIndex(uint8 Index)
{
    PlayerIndex = Index;
}

uint8 AProjekt994BeaconClient::GetPlayerIndex()
{
    return PlayerIndex;
}