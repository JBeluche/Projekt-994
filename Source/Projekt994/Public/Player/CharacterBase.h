// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class PROJEKT994_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;


	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, Category = "Projekt994 Settings")
		TSubclassOf<class AWeaponBase> StartingWeaponClass;

	UPROPERTY(ReplicatedUsing = OnRep_AttachWeapon)
		class AWeaponBase* CurrentWeapon;
	UFUNCTION()
		void OnRep_AttachWeapon();	

	int32 WeaponIndex;
	TArray<AWeaponBase*> WeaponArray;

	//Set to replicate, skip owner
	UPROPERTY(Replicated)
	bool bIsAiming;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetAiming(bool WantsToAim);
		bool Server_SetAiming_Validate(bool WantsToAim);
		void Server_SetAiming_Implementation(bool WantsToAim);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	
	/** Fires a projectile. */
	virtual void OnFire();

	virtual void OnAimingStart();
	virtual void OnAimingEnd();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/** Called when turning. */
	void TurnAtRate(float Rate);

	/** Called when looking up/down. */
	void LookUpAtRate(float Rate);

public:
	UFUNCTION(BlueprintCallable)
		bool GetIsAiming();

	UFUNCTION(BlueprintCallable)
		class AWeaponBase* GetCurrentWeapon();

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};
