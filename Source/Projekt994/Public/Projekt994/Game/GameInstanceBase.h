// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Http.h"

#include "GameInstanceBase.generated.h"


USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString MapURL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString MapName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString MapDescription;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UTexture2D* MapImage;
};

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int ServerID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString IPAddress;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString ServerName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString MapName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int CurrentPlayers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int MaxPlayers;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FServersReceived);

UCLASS()
class PROJEKT994_API UGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UGameInstanceBase();

//Http
protected:
	FHttpModule* Http;	

	TArray<FServerData> ServerList;

	UFUNCTION(BlueprintCallable)
		TArray<FServerData>& GetServerList();

	UPROPERTY(BlueprintAssignable)
		FServersReceived FOnServersReceived;

	UFUNCTION(BlueprintCallable)
		void GenerateServerList();

	void OnServerListRequestComplete(FHttpRequestPtr Request,FHttpResponsePtr Response, bool Success);

protected:
	UFUNCTION(BlueprintCallable)
		void GoToMap(FString MAPURL);


};
