// Copyright by Creating Mountains

//Custom
#include "Projekt994/Public/Projekt994/Zombie/AIControllerZombieBase.h"
#include "Projekt994/Public/Projekt994/Game/AttackZoneTriggerCapsule.h"

//Engine
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"


AAIControllerZombieBase::AAIControllerZombieBase()
{

}

void AAIControllerZombieBase::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Error, TEXT("running behavior tree"));

    if(AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
    }
}


void AAIControllerZombieBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if(LineOfSightTo(PlayerPawn))
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    }
    else
    {
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }

    /*if(GetClosestAttackZone()->GetActor()))   
    {
        

    }
    else
    {
        GetBlackboardComponent()->SetValueAsBool(TEXT("IsInAttackZone"), false);
        GetBlackboardComponent()->ClearValue(TEXT("IsInAttackZone"));
    }
    */
     if(GetPawn())
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("AttackZoneLocation"), GetClosestAttackZone()->GetActorLocation());
    }
    //if owner is in triggerbox
    //set blackboard key
}


class AAttackZoneTriggerCapsule*  AAIControllerZombieBase::GetClosestAttackZone()
{
 	float GuardVal = 10000.f; // This value will allow you to santize your result.
    FVector ZombieLocation = GetPawn()->GetActorLocation(); // Store your current location. Saves on calls.
    AAttackZoneTriggerCapsule* ClosestAZ = nullptr; // we will set this if we find the closest actor.
	
	for (TActorIterator<AAttackZoneTriggerCapsule> AttackZoneIt(GetWorld(), AAttackZoneTriggerCapsule::StaticClass());  AttackZoneIt; ++AttackZoneIt)
	{
		AAttackZoneTriggerCapsule* AZ = *AttackZoneIt;
		if(AZ != NULL)
		{
			// store a value that is your location minus the box locaiton. Then get the length of that vector
        	float delta = (ZombieLocation - AZ->GetActorLocation()).Size();
			 if(delta < GuardVal) // if it is the shortest.
			{
				GuardVal = delta; // store the new closest.
				ClosestAZ = AZ; // store a reference to the box we found.
			}
		}
	}
	
	return ClosestAZ;
}

void AAIControllerZombieBase::SetIsInAttackZone()
{
        UE_LOG(LogTemp, Warning, TEXT("setting"));

    GetBlackboardComponent()->SetValueAsBool(TEXT("IsInAttackZone"), true);
}

void AAIControllerZombieBase::ClearIsInAttackZone()
{
    GetBlackboardComponent()->SetValueAsBool(TEXT("IsInAttackZone"), false);
}

