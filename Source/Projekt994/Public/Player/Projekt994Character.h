// Copyright by Creating Mountains

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "Projekt994Character.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractChanged, const FString&, OnInteractChanged);

UCLASS()
class PROJEKT994_API AProjekt994Character : public ACharacterBase
{
	GENERATED_BODY()

public:
	AProjekt994Character();
	
protected:
	UPROPERTY(BlueprintAssignable)
		FInteractChanged OnInteractChanged;


	FTimerHandle TInteractTimerHandle;
	class AInteractableBase* Interactable;

	UPROPERTY(EditDefaultsOnly)
		float InteractionRange;

	UPROPERTY(EditDefaultsOnly)//Set to replicate
		uint32 Points;

protected:
	void Interact();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Interact(class AInteractableBase* InteractingObject);
		bool Server_Interact_Validate(class AInteractableBase* InteractingObject);
		void Server_Interact_Implementation(class AInteractableBase* InteractingObject);

	void SetInteractableObject();

	virtual void OnFire() override;

public: 
	void IncrementPoints(uint16 Value);
	bool DecrementPoints(uint16 Value);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
