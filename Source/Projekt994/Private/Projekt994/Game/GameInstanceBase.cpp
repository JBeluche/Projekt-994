// Fill out your copyright notice in the Description page of Project Settings.


#include "Projekt994/Public/Projekt994/Game/GameInstanceBase.h"
#include "JsonObjectConverter.h"

#include "Engine/World.h"

UGameInstanceBase::UGameInstanceBase()
{
    Http = &FHttpModule::Get();

}

void UGameInstanceBase::GoToMap(FString MAPURL)
{
    GetWorld()->ServerTravel(MAPURL);
    
}

void UGameInstanceBase::GenerateServerList()
{
    //Remove server db entry
    TSharedRef<IHttpRequest> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &UGameInstanceBase::OnServerListRequestComplete);

    Request->SetURL("https://localhost:44344/api/Host");
    Request->SetVerb("GET");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->ProcessRequest();
}

void UGameInstanceBase::OnServerListRequestComplete(FHttpRequestPtr Request,FHttpResponsePtr Response, bool Success)
{
    if (Success)
    {
        FString ResponseStr = Response->GetContentAsString();
        ResponseStr.InsertAt(0, FString("{\"Response\":"));
        ResponseStr.AppendChar('}');
        UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *ResponseStr);

        TSharedPtr<FJsonObject> JsonObject= MakeShareable(new FJsonObject);
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseStr);

        if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
        {
            TArray<TSharedPtr<FJsonValue>> JsonValues = JsonObject->GetArrayField(TEXT("Response"));

            if (JsonValues.Num() > 0)
            {
                ServerList.Empty();
            }

            for (TSharedPtr<FJsonValue> Value : JsonValues)
            {
                FServerData ServerData = FServerData();
                TSharedPtr<FJsonObject> JsonObj = Value->AsObject();

                ServerData.ServerName = JsonObj->GetStringField("ServerName");
                ServerData.IPAddress = JsonObj->GetStringField("IPAddress");
                ServerData.MapName = JsonObj->GetStringField("MapName");
                ServerData.CurrentPlayers = JsonObj->GetIntegerField("CurrentPlayers");
                ServerData.MaxPlayers = JsonObj->GetIntegerField("MaxPlayers");

                ServerList.Add(ServerData);
              
            }

            FOnServersReceived.Broadcast();

           /* for (FServerData ServerData : ServerList)
            {
                    UE_LOG(LogTemp, Warning, TEXT("IP: %s"), *ServerData.IPAddress);
                    UE_LOG(LogTemp, Warning, TEXT("ServerName: %s"), *ServerData.ServerName);
                    UE_LOG(LogTemp, Warning, TEXT("MapNAme: %s"), *ServerData.MapName);
                    UE_LOG(LogTemp, Warning, TEXT("CurrentPlayers: %d"), ServerData.CurrentPlayers);
                    UE_LOG(LogTemp, Warning, TEXT("MaxPlayers: %d"), ServerData.MaxPlayers);
                    UE_LOG(LogTemp, Warning, TEXT("End of server entry"));

            }*/
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Instance base could not GET on api, GetServerList"));
    }
}


TArray<FServerData>& UGameInstanceBase::GetServerList()
{
    return ServerList;
}




 