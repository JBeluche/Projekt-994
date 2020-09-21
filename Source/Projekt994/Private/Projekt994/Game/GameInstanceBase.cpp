// Fill out your copyright notice in the Description page of Project Settings.


#include "Projekt994/Public/Projekt994/Game/GameInstanceBase.h"

#include "Engine/World.h"

UGameInstanceBase::UGameInstanceBase()
{

}

void UGameInstanceBase::GoToMap(FString MAPURL)
{
    GetWorld()->ServerTravel(MAPURL);
}