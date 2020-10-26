// Copyright by Creating Mountains


#include "Projekt994/Public/Player/Projekt994Character.h"
#include "TimerManager.h"
#include "Projekt994/Public/Projekt994//Useables/InteractableBase.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

AProjekt994Character::AProjekt994Character()
{
	Interactable = nullptr;

}

// Called when the game starts or when spawned
void AProjekt994Character::BeginPlay()
{
	Super::BeginPlay();
	
		//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

    GetWorld()->GetTimerManager().SetTimer(TInteractTimerHandle, this, &AProjekt994Character::Interact, 0.2f, true);
}

//////////////////////////////////////////////////////////////////////////
// Input
// Called to bind functionality to input
void AProjekt994Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AProjekt994Character::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AProjekt994Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjekt994Character::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProjekt994Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProjekt994Character::LookUpAtRate);

}

void AProjekt994Character::Interact()
{
    FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
    FVector Rot = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
    FVector End = Start + Rot * 5000.0f;


    FHitResult HitResult;
    FCollisionObjectQueryParams CollisionQuery;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionQuery, CollisionParams);

    if (AInteractableBase* Temp = Cast<AInteractableBase>(HitResult.GetActor()))
    {
        Interactable = Temp;
        UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR %s"), *HitResult.GetActor()->GetName());
    }
    else
    {
        Interactable = nullptr;

        UE_LOG(LogTemp, Warning, TEXT("Did not hit anything"));

    }
}
