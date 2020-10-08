// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Projekt994/Public/Projekt994/MainMenu/Projekt994BeaconHostObject.h"
#include "OnlineBeaconClient.h"
#include "Projekt994BeaconClient.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccess, bool, FOnConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyUpdated, FProjekt994LobbyInfo, FOnLobbyUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChatRecieved, const FText&, FOnChatRecieved);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFullConnect);

UCLASS()
class PROJEKT994_API AProjekt994BeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()

public:
	AProjekt994BeaconClient();

//Blueprint events
protected:
	UPROPERTY(BlueprintAssignable)
		FConnectSuccess FOnConnected;
	UPROPERTY(BlueprintAssignable)
		FDisconnected FOnDisconnected;
		
	UPROPERTY(BlueprintAssignable)
		FLobbyUpdated FOnLobbyUpdated;

	UPROPERTY(BlueprintAssignable)
		FChatRecieved FOnChatRecieved;

	UPROPERTY(BlueprintAssignable)
		FFullConnect FOnFullConnect;

		uint8 PlayerIndex;
		FString PlayerName;

//Connections
protected:
	UFUNCTION(BlueprintCallable)
		bool ConnectToServer(const FString& Address);

	UFUNCTION(BlueprintCallable)
		void FullConnectToServer(const FString& JoinAddress);

	UFUNCTION(BlueprintCallable)
		void LeaveLobby();

	virtual void OnFailure() override;
	virtual void OnConnected() override;

//Chat
protected: 
	UFUNCTION(BlueprintCallable)
		void SendChatMessage(const FText& ChatMessage);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendChatMessage(const FText& ChatMessage);
		bool Server_SendChatMessage_Validate(const FText& ChatMessage);
		void Server_SendChatMessage_Implementation(const FText& ChatMessage);

//Connections, Lobby and Chat
public:
	UFUNCTION(Client, Reliable)
		void Client_OnDisconnected();
	virtual void Client_OnDisconnected_Implementation();

	UFUNCTION(Client, Reliable)
		void Client_FullConnect();
	virtual void Client_FullConnect_Implementation();

	UFUNCTION(Client, Reliable)
		void Client_OnLobbyUpdated(FProjekt994LobbyInfo LobbyInfo);
	void Client_OnLobbyUpdated_Implementation(FProjekt994LobbyInfo LobbyInfo);

	UFUNCTION(Client, Reliable)
		void Client_OnChatMessageRecieved(const FText& ChatMessage);
		void Client_OnChatMessageRecieved_Implementation(const FText& ChatMessage);


	void SetPlayerIndex(uint8 Index);
	uint8 GetPlayerIndex();

	void SetPlayerName(const FString& NewPlayerName);
	FString GetPlayerName();
	
};
