// Copyright by Creating Mountains

#include "Projekt994/Public/Player/Projekt994Character.h"
#include "TimerManager.h"
#include "Projekt994/Public/Projekt994//Useables/InteractableBase.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

AProjekt994Character::AProjekt994Character()
{
    Interactable = nullptr;
    InteractionRange = 200.0f;
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
            Interactable->Use(this);
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
