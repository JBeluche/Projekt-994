// Copyright by Creating Mountains

#pragma once

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


UCLASS()
class PROJEKT994_API AProjekt994BeaconHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()
	
public:
	AProjekt994BeaconHostObject();

protected:
	FProjekt994LobbyInfo LobbyInfo;

		UPROPERTY(BlueprintAssignable)
		FHostLobbyUpdated FOnHostLobbyUpdated;

	UFUNCTION(BlueprintCallable)
		void UpdateLobbyInfo(FProjekt994LobbyInfo NewLobbyInfo);

	void UpdateClientLobbyInfo();


protected:
	virtual void BeginPlay() override;

	virtual void OnClientConnected(AOnlineBeaconClient * NewClientActor,UNetConnection * ClientConnection) override;
	virtual void NotifyClientDisconnected(AOnlineBeaconClient * LeavingClientActor) override;

	UFUNCTION(BlueprintCallable)
		void ShutdownServer();

		void DisconnectAllClients();

		virtual void DisconnectClient(AOnlineBeaconClient* ClientActor) override;

public:
	void SendChatToLobby(const FText& ChatMessage);	
};
