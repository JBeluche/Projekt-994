// Copyright by Creating Mountains

#include "Projekt994/Public/Player/CharacterBase.h"

#include "Projekt994/Public/Projekt994//Useables/InteractableBase.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
	ObjectName = "Default";
	UIMessage = "Press F to buy " + ObjectName;

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

void AInteractableBase::Use(AProjekt994Character* Player)
{

}

