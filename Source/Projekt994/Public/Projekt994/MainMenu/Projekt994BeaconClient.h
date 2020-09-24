// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "Projekt994BeaconClient.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccess, bool, FConnected);

UCLASS()
class PROJEKT994_API AProjekt994BeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()

public:
	AProjekt994BeaconClient();

protected:
	UPROPERTY(BlueprintAssignable)
		FConnectSuccess FConnected;

protected:
	UFUNCTION(BlueprintCallable)
		bool ConnectToServer(const FString& Address);

	virtual void OnFailure() override;
	virtual void OnConnected() override;
	
};
