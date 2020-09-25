// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Projekt994/Public/Projekt994/MainMenu/Projekt994BeaconHostObject.h"
#include "OnlineBeaconClient.h"
#include "Projekt994BeaconClient.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccess, bool, FOnConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyUpdated, FProjekt994LobbyInfo, FOnLobbyUpdated);


UCLASS()
class PROJEKT994_API AProjekt994BeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()

public:
	AProjekt994BeaconClient();

protected:
	UPROPERTY(BlueprintAssignable)
		FConnectSuccess FOnConnected;
	UPROPERTY(BlueprintAssignable)
		FDisconnected FOnDisconnected;
		
	UPROPERTY(BlueprintAssignable)
		FLobbyUpdated FOnLobbyUpdated;

protected:
	UFUNCTION(BlueprintCallable)
		bool ConnectToServer(const FString& Address);

	UFUNCTION(BlueprintCallable)
		void LeaveLobby();

	virtual void OnFailure() override;
	virtual void OnConnected() override;

public:
	UFUNCTION(Client, Reliable)
		void Client_OnDisconnected();
	virtual void Client_OnDisconnected_Implementation();

	UFUNCTION(Client, Reliable)
		void Client_OnLobbyUpdated(FProjekt994LobbyInfo LobbyInfo);
	void Client_OnLobbyUpdated_Implementation(FProjekt994LobbyInfo LobbyInfo);
	
};
