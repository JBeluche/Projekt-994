// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994/MainMenu/Projekt994BeaconHostObject.h"
#include "Projekt994/Public/Projekt994/MainMenu/Projekt994BeaconClient.h"
#include "Projekt994/Public/Projekt994/MainMenu/Projekt994MainMenuGameMode.h"
#include "Projekt994/Public/Projekt994/Game/GameInstanceBase.h"

#include "OnlineBeaconHost.h"
#include "TimerManager.h"

AProjekt994BeaconHostObject::AProjekt994BeaconHostObject()
{
    ClientBeaconActorClass = AProjekt994BeaconClient::StaticClass();
    BeaconTypeName = ClientBeaconActorClass->GetName();

    Http = &FHttpModule::Get();
    ServerID = -1;
}

void AProjekt994BeaconHostObject::OnClientConnected(AOnlineBeaconClient *NewClientActor, UNetConnection *ClientConnection)
{
    Super::OnClientConnected(NewClientActor, ClientConnection);

    if (NewClientActor)
    {

        if (GetCurrentPlayerCount() >= ServerData.MaxPlayers)
        {
            if (AProjekt994BeaconClient *Client = Cast<AProjekt994BeaconClient>(NewClientActor))
            {
                Client->SetPlayerIndex(240);
                DisconnectClient(NewClientActor);
            return;
            }
        }

        FString PlayerName = FString("Player ");
        uint8 Index = LobbyInfo.PlayerList.Num();
        PlayerName.Append(FString::FromInt(Index));
        LobbyInfo.PlayerList.Add(PlayerName);

        if (AProjekt994BeaconClient *Client = Cast<AProjekt994BeaconClient>(NewClientActor))
        {
            Client->SetPlayerIndex(Index);
            Client->SetPlayerName(PlayerName);
        }

        UE_LOG(LogTemp, Warning, TEXT("Connect to client VALID"))
        FOnHostLobbyUpdated.Broadcast(LobbyInfo);

        UpdateClientLobbyInfo();

        UpdateServerData(ServerData);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Connect to client INVALID"))
    }
}

void AProjekt994BeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient *LeavingClientActor)
{
    Super::NotifyClientDisconnected(LeavingClientActor);

    UE_LOG(LogTemp, Warning, TEXT("Client has disconnected"));

    if (AProjekt994BeaconClient *Client = Cast<AProjekt994BeaconClient>(LeavingClientActor))
    {
        uint8 Index = Client->GetPlayerIndex();
        if (Index == 240)
        {
            return;
        }

        LobbyInfo.PlayerList.RemoveAt(Index);
    }
    FOnHostLobbyUpdated.Broadcast(LobbyInfo);

    UpdateServerData(ServerData);
    UpdateClientLobbyInfo();
}

void AProjekt994BeaconHostObject::ShutdownServer()
{
    //Unregister Server From Database Via Web API
    DisconnectAllClients();

    if (AOnlineBeaconHost *Host = Cast<AOnlineBeaconHost>(GetOwner()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Host has been destroyed"));
        Host->UnregisterHost(BeaconTypeName);
        Host->DestroyBeacon();
    }

    if (ServerID != -1)
    {

        //Remove server db entry
        TSharedRef<IHttpRequest> Request = Http->CreateRequest();

        Request->SetURL("https://localhost:44344/api/Host/" + FString::FromInt(ServerID));
        Request->SetVerb("DELETE");
        Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
        Request->ProcessRequest();
    }
}

void AProjekt994BeaconHostObject::DisconnectAllClients()
{
    UE_LOG(LogTemp, Warning, TEXT("Disconecting all clients"));

    for (AOnlineBeaconClient *Client : ClientActors)
    {
        if (Client)
        {
            DisconnectClient(Client);
        }
    }
}

void AProjekt994BeaconHostObject::DisconnectClient(AOnlineBeaconClient *ClientActor)
{

    AOnlineBeaconHost *BeaconHost = Cast<AOnlineBeaconHost>(GetOwner());
    if (BeaconHost)
    {
        if (AProjekt994BeaconClient *Client = Cast<AProjekt994BeaconClient>(ClientActor))
        {
            UE_LOG(LogTemp, Warning, TEXT("Disconecting client: %s"), *ClientActor->GetName());
            Client->Client_OnDisconnected();
        }
        BeaconHost->DisconnectClient(ClientActor);
    }
        UpdateServerData(ServerData);

}

void AProjekt994BeaconHostObject::UpdateLobbyInfo(FProjekt994LobbyInfo NewLobbyInfo)
{
    LobbyInfo.MapImage = NewLobbyInfo.MapImage;
    UpdateClientLobbyInfo();
    FOnHostLobbyUpdated.Broadcast(LobbyInfo);
}

void AProjekt994BeaconHostObject::UpdateClientLobbyInfo()
{
    for (AOnlineBeaconClient *ClientBeacon : ClientActors)
    {
        if (AProjekt994BeaconClient *Client = Cast<AProjekt994BeaconClient>(ClientBeacon))
        {
            Client->Client_OnLobbyUpdated(LobbyInfo);
        }
    }
}

void AProjekt994BeaconHostObject::BeginPlay()
{
    LobbyInfo.PlayerList.Add(FString("Host"));
    GetWorld()->GetTimerManager().SetTimer(TInitialLobbyHandle, this, &AProjekt994BeaconHostObject::InitialLobbyHandling, 0.2f, false);
}

void AProjekt994BeaconHostObject::SendChatToLobby(const FText &ChatMessage)
{
    FOnHostChatRecieved.Broadcast(ChatMessage);
    for (AOnlineBeaconClient *ClientBeacon : ClientActors)
    {
        if (AProjekt994BeaconClient *Client = Cast<AProjekt994BeaconClient>(ClientBeacon))
        {
            Client->Client_OnChatMessageRecieved(ChatMessage);
        }
    }
}

void AProjekt994BeaconHostObject::InitialLobbyHandling()
{
    UpdateLobbyInfo(LobbyInfo);
}

void AProjekt994BeaconHostObject::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
    if (Success)
    {
        ServerID = FCString::Atoi(*Response->GetContentAsString());
        UE_LOG(LogTemp, Warning, TEXT("HttpRequest Success: %d"), ServerID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HttpRequest FAILED, Haha noob"));
    }
}

//SET SERVER DATA
void AProjekt994BeaconHostObject::SetServerData(FServerData NewServerData)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    ServerData = NewServerData;
    ServerData.CurrentPlayers = GetCurrentPlayerCount();

    JsonObject->SetNumberField("ServerID", 0);
    JsonObject->SetStringField("IPAddress", "127.69");
    JsonObject->SetStringField("ServerName", ServerData.ServerName);
    JsonObject->SetStringField("MapName", ServerData.MapName);
    JsonObject->SetNumberField("CurrentPlayers", ServerData.CurrentPlayers);
    JsonObject->SetNumberField("MaxPlayers", ServerData.MaxPlayers);

    FString JsonString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &AProjekt994BeaconHostObject::OnProcessRequestComplete);

    Request->SetURL("https://localhost:44344/api/Host");
    Request->SetVerb("POST");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(JsonString);
    Request->ProcessRequest();
}

//SET SERVER DATA
void AProjekt994BeaconHostObject::UpdateServerData(FServerData NewServerData)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    ServerData = NewServerData;
    ServerData.CurrentPlayers = GetCurrentPlayerCount();

    JsonObject->SetNumberField("ServerID", 0);
    JsonObject->SetStringField("IPAddress", "127.69");
    JsonObject->SetStringField("ServerName", ServerData.ServerName);
    JsonObject->SetStringField("MapName", ServerData.MapName);
    JsonObject->SetNumberField("CurrentPlayers", ServerData.CurrentPlayers);
    JsonObject->SetNumberField("MaxPlayers", ServerData.MaxPlayers);

    FString JsonString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &AProjekt994BeaconHostObject::OnProcessRequestComplete);

    Request->SetURL("https://localhost:44344/api/Host/1");
    Request->SetVerb("PUT");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(JsonString);
    Request->ProcessRequest();
}

int AProjekt994BeaconHostObject::GetCurrentPlayerCount()
{
    return LobbyInfo.PlayerList.Num();
}
