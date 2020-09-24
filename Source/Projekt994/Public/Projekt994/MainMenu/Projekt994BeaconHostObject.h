// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "Projekt994BeaconHostObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API AProjekt994BeaconHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()
	
public:
	AProjekt994BeaconHostObject();

protected:

	virtual void OnClientConnected(AOnlineBeaconClient * NewClientActor,UNetConnection * ClientConnection) override;
	virtual void NotifyClientDisconnected(AOnlineBeaconClient * LeavingClientActor) override;

	UFUNCTION(BlueprintCallable)
		void ShutdownServer();
		
		void DisconnectAllClients();
};
