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
		class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		class UStaticMeshComponent* CollisionMesh;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		class UAnimationAsset* OpenAnimation;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		int32 Cost;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		uint8 AccessZone;

	UPROPERTY(ReplicatedUsing = OnRep_BarricadeUsed)
		bool bIsUsed;

	UFUNCTION()
		void OnRep_BarricadeUsed();


protected:
	virtual void BeginPlay() override;
	virtual void Use(class AProjekt994Character* Player) override;

public:
	uint8 GetAccessZone();

};
