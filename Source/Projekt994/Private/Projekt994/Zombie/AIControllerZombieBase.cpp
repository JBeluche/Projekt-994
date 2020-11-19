// Copyright by Creating Mountains


#include "Projekt994/Public/Projekt994/Zombie/AIControllerZombieBase.h"

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