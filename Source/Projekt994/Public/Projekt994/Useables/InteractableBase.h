// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

UCLASS()
class PROJEKT994_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		FString UIMessage;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		FString ObjectName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FString GetUIMessage();

	virtual void Use(class ACharacterBase* Player);


};
