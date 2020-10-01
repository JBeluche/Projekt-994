// Fill out your copyright notice in the Description page of Project Settings.


#include "Projekt994/Public/Projekt994/Game/GameInstanceBase.h"

#include "Engine/World.h"

UGameInstanceBase::UGameInstanceBase()
{
    Http = &FHttpModule::Get();

}

void UGameInstanceBase::GoToMap(FString MAPURL)
{
    GetWorld()->ServerTravel(MAPURL);
    
}

void UGameInstanceBase::GetServerList()
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
        UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Response->GetContentAsString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Instance base could not GET on api, GetServerList"));
    }
}



 