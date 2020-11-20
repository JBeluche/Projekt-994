// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994/Game/AttackZoneTriggerCapsule.h"
#include "Projekt994/Public/Projekt994/Zombie/AIControllerZombieBase.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"

AAttackZoneTriggerCapsule::AAttackZoneTriggerCapsule()
{
    OnActorBeginOverlap.AddDynamic(this, &AAttackZoneTriggerCapsule::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AAttackZoneTriggerCapsule::OnOverlapEnd);
}

void AAttackZoneTriggerCapsule::BeginPlay()
{
}

void AAttackZoneTriggerCapsule::OnOverlapBegin(class AActor *OverlappedActor, class AActor *OtherActor)
{
    UE_LOG(LogTemp, Warning, TEXT("casting with: %s!!"), *OtherActor->GetName());

    if (AZombieBase *Zombie = Cast<AZombieBase>(OtherActor))
    {
        if (AAIControllerZombieBase *ZombieController = Zombie->GetController<AAIControllerZombieBase>())
		{
            UE_LOG(LogTemp, Warning, TEXT("casting success!! "));

            ZombieController->SetIsInAttackZone();

		}
    }
}

void AAttackZoneTriggerCapsule::OnOverlapEnd(class AActor *OverlappedActor, class AActor *OtherActor)
{
    /*
        if (AAIControllerZombieBase *ZombieController = Cast<AAIControllerZombieBase>(OverlappedActor))
    {
        ZombieController->ClearIsInAttackZone();
    }*/
}
