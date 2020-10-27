// Copyright by Creating Mountains

#include "Projekt994/Public/Player/Projekt994Character.h"
#include "TimerManager.h"
#include "Projekt994/Public/Projekt994/Useables/InteractableBase.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"
#include "DrawDebugHelpers.h"



AProjekt994Character::AProjekt994Character()
{
    Interactable = nullptr;
    InteractionRange = 200.0f;
    Points = 500;

}

// Called when the game starts or when spawned
void AProjekt994Character::BeginPlay()
{
    Super::BeginPlay();

    //Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
    FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

    GetWorld()->GetTimerManager().SetTimer(TInteractTimerHandle, this, &AProjekt994Character::SetInteractableObject, 0.2f, true);
}

//////////////////////////////////////////////////////////////////////////
// Input
// Called to bind functionality to input
void AProjekt994Character::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AProjekt994Character::Interact);
}
void AProjekt994Character::Interact()
{
        if (Interactable)
        {
            if(HasAuthority())
            {
                Interactable->Use(this);
            }
            else
            {
                Server_Interact(Interactable);
            }
            
        }
}

void AProjekt994Character::SetInteractableObject()
{
    FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
    FVector Rot = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
    FVector End = Start + Rot * InteractionRange;

    FHitResult HitResult;
    FCollisionObjectQueryParams CollisionQuery;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionQuery, CollisionParams);

    AInteractableBase *Temp = Cast<AInteractableBase>(HitResult.GetActor());
    if (Interactable == nullptr && Temp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Is now a valid ptr"));
        Interactable = Temp;
        OnInteractChanged.Broadcast(Interactable->GetUIMessage());

    }
    else if (Interactable && Temp == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Is now a nullptr"));
        Interactable = nullptr;
        OnInteractChanged.Broadcast(FString());

    }
}

bool AProjekt994Character::Server_Interact_Validate(AInteractableBase* InteractingObject)
{
    return true;
}

void AProjekt994Character::Server_Interact_Implementation(AInteractableBase* InteractingObject)
{
    float Distance = GetDistanceTo(InteractingObject);
    if(Distance < InteractionRange + 30.0f)
    {
        InteractingObject->Use(this);
    }
}

void AProjekt994Character::OnFire()
{
    FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
    FVector Rot = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
    FVector End = Start + Rot * 2000.0f;

    FHitResult HitResult;
    FCollisionObjectQueryParams CollisionQuery;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    if(GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionQuery, CollisionParams))
    {
        if (AZombieBase* Zombie = Cast<AZombieBase>(HitResult.GetActor()))
        {
            UE_LOG(LogTemp, Warning, TEXT("Zombie hit@!@ %s"), *Zombie->GetName());
            Zombie->Hit(this);
        }
    }

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);

}

void AProjekt994Character::IncrementPoints(uint16 Value)
{
    Points += Value;
    UE_LOG(LogTemp, Warning, TEXT("Zombie hit@!@ %d"), Points);

}

bool AProjekt994Character::DecrementPoints(uint16 Value)
{
    if(Points - Value < 0)
    {
        return false;
    }
    else
    {
        Points -= Value;
        return true;
    }
    UE_LOG(LogTemp, Warning, TEXT("Zombie hit@!@ %d"), Points);
    
}
