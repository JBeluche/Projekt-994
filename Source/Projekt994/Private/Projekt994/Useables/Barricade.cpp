// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994//Useables/Barricade.h"

#include "Components/StaticMeshComponent.h"

ABarricade::ABarricade()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

    RootComponent = MeshComp;
    Cost = 500;

    UIMessage += ObjectName + " [Cost: " + FString::FromInt(Cost) + "]";


}

void ABarricade::BeginPlay()
{
    Super::BeginPlay();

}

