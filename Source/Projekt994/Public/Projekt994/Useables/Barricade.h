// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Projekt994/Useables/InteractableBase.h"
#include "Barricade.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT994_API ABarricade : public AInteractableBase
{
	GENERATED_BODY()

public:
	ABarricade();

protected:
	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		uint16 Cost;



protected:
	virtual void BeginPlay() override;
};
