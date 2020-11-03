// Fill out your copyright notice in the Description page of Project Settings.

#include "Projekt994/Public/Player/CharacterBase.h"
#include "Projekt994/Projekt994Projectile.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Projekt994/Public/Projekt994//Useables/WeaponBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	WeaponIndex = 0;
	bIsAiming = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();


	if (HasAuthority())
	{
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(StartingWeaponClass);
		//Spawn Weapon using statringweaponclass
		if (CurrentWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawned weapon and attempted to attach to hand socket"));

			WeaponArray.Add(CurrentWeapon);
			OnRep_AttachWeapon();
		}
	}

	//Attach spawn weapon to sockets_weaponSocket

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, CurrentWeapon);
} 

void ACharacterBase::OnRep_AttachWeapon()
{
	if (CurrentWeapon)
	{
		if (IsLocallyControlled())
		{
			CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("s_weaponSocket"));
		}
		else
		{
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("s_weaponSocket"));
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Input
// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ACharacterBase::OnAimingStart);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ACharacterBase::OnAimingEnd);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterBase::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacterBase::LookUpAtRate);

	WeaponIndex = 0;
}

void ACharacterBase::OnFire()
{
}

void ACharacterBase::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACharacterBase::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACharacterBase::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ACharacterBase::GetIsAiming()
{
	return bIsAiming;
}

void ACharacterBase::OnAimingStart()
{
	bIsAiming = true;
}
void ACharacterBase::OnAimingEnd()
{
	bIsAiming = false;
}


