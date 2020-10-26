// Copyright by Creating Mountains


#include "Projekt994/Public/Player/Projekt994Character.h"

AProjekt994Character::AProjekt994Character()
{

}

// Called when the game starts or when spawned
void AProjekt994Character::BeginPlay()
{
	Super::BeginPlay();
	
		//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
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