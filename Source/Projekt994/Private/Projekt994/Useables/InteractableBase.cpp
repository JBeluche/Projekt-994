// Copyright by Creating Mountains

#include "Projekt994/Public/Player/CharacterBase.h"

#include "Projekt994/Public/Projekt994//Useables/InteractableBase.h"

// Sets default values
AInteractableBase::AInteractableBase()
{

	UIMessage = ":Press and hold F to purchase";

}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FString  AInteractableBase::GetUIMessage()
{
	return UIMessage;
}

void AInteractableBase::Use(ACharacterBase* Player)
{

}

