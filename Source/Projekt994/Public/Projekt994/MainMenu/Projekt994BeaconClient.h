// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "Projekt994BeaconClient.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccess, bool, FOnConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnected);

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
	
};
