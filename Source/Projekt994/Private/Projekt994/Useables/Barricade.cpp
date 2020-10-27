// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994//Useables/Barricade.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

ABarricade::ABarricade()
{
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    RootComponent = MeshComp;

    CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

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
    CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if(OpenAnimation)
    {
        MeshComp->PlayAnimation(OpenAnimation, false);
    }
    
}
