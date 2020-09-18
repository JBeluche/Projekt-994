// Copyright Epic Games, Inc. All Rights Reserved.

#include "Projekt994GameMode.h"
#include "Projekt994HUD.h"
#include "Projekt994Character.h"
#include "UObject/ConstructorHelpers.h"

AProjekt994GameMode::AProjekt994GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintClasses/Player/BP_CharacterBase.BP_CharacterBase"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AProjekt994HUD::StaticClass();
}
