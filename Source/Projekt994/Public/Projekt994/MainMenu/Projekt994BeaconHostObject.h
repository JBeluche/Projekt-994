// Copyright by Creating Mountains

#pragma once

#include "Http.h"
#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "Projekt994BeaconHostObject.generated.h"


USTRUCT(BlueprintType)
struct FProjekt994LobbyInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		class UTexture2D* MapImage;

	UPROPERTY(BlueprintReadOnly)
		TArray<FString> PlayerList;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHostLobbyUpdated, FProjekt994LobbyInfo, FOnHostLobbyUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHostChatRecieved, const FText&, FOnHostChatRecieved);

UCLASS()
class PROJEKT994_API AProjekt994BeaconHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()
	
public:
	AProjekt994BeaconHostObject();

//Http
protected:
	FHttpModule* Http;	
	int ServerID;
	void OnProcessRequestComplete(FHttpRequestPtr Request,FHttpResponsePtr Response, bool Success);

//Lobby 
protected:
	FProjekt994LobbyInfo LobbyInfo;

	UFUNCTION(BlueprintCallable)
		void SetServerData(const FString ServerName, const FString MapName, int CurrentPlayers, int MaxPlayers);

	UFUNCTION(BlueprintCallable)
		int GetCurrentPlayerCount();

	UPROPERTY(BlueprintAssignable)
		FHostLobbyUpdated FOnHostLobbyUpdated;

		
	UPROPERTY(BlueprintAssignable)
		FHostChatRecieved FOnHostChatRecieved;

	FTimerHandle TInitialLobbyHandle;
		void InitialLobbyHandling();

	UFUNCTION(BlueprintCallable)
		void UpdateLobbyInfo(FProjekt994LobbyInfo NewLobbyInfo);

	void UpdateClientLobbyInfo();

//Connections
protected:
	virtual void BeginPlay() override;

	virtual void OnClientConnected(AOnlineBeaconClient * NewClientActor,UNetConnection * ClientConnection) override;
	virtual void NotifyClientDisconnected(AOnlineBeaconClient * LeavingClientActor) override;

	UFUNCTION(BlueprintCallable)
		void ShutdownServer();

		void DisconnectAllClients();

		virtual void DisconnectClient(AOnlineBeaconClient* ClientActor) override;

public:
	UFUNCTION(BlueprintCallable)
	void SendChatToLobby(const FText& ChatMessage);	
};
