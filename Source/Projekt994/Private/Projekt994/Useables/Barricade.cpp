// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994//Useables/Barricade.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ABarricade::ABarricade()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

    RootComponent = MeshComp;
    Cost = 500;

    UIMessage += ObjectName + " [Cost: " + FString::FromInt(Cost) + "]";
    bIsUsed = false;
}

void ABarricade::BeginPlay()
{
    Super::BeginPlay();
    SetReplicates(true);
}
void ABarricade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABarricade, bIsUsed);
}

void ABarricade::Use(ACharacterBase *Player)
{
    UE_LOG(LogTemp, Warning, TEXT("IN USE FUNCTION %s"), *GetName());
    bIsUsed = true;
    OnRep_BarricadeUsed();
}

void ABarricade::OnRep_BarricadeUsed()
{
    SetActorEnableCollision(false);
    
}
