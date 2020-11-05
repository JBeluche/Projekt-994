// Copyright by Creating Mountains

#include "Projekt994/Public/Player/Projekt994PlayerState.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994GameModeBase.h"
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
    AccessZone = 0;

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

void ABarricade::Use(AProjekt994Character *Player)
{
    if(HasAuthority() && !bIsUsed && Player)
    {
		if(AProjekt994PlayerState* PState = Player->GetPlayerState<AProjekt994PlayerState>())
        {
            if(!PState->DecrementPoints(Cost))
            {   
                return;
            }

            bIsUsed = true;
            OnRep_BarricadeUsed();

            if (AProjekt994GameModeBase* GM = GetWorld()->GetAuthGameMode<AProjekt994GameModeBase>())
            {
                GM->NewZoneActive(AccessZone);
            }
        }

    }
   
}

void ABarricade::OnRep_BarricadeUsed()
{
    CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if(OpenAnimation)
    {
        MeshComp->PlayAnimation(OpenAnimation, false);
        UIMessage = FString();
    }
    
}

uint8 ABarricade::GetAccessZone()
{
    return AccessZone;
}
