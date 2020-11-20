// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Unreal_CPPCharacter.generated.h"

UCLASS(config=Game)
class AUnreal_CPPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AUnreal_CPPCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Animation")
	bool isStrafing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Animation")
	bool isGoingRight;

	UCharacterMovementComponent* movement = nullptr;

private:
	/* Pick Up & Drop */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PickUp")
	float pickupDistance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PickUp")
	bool isHoldingObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "PickUp")
	class UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "PickUp")
	class USceneComponent* GrabLocation;

	/* Respawn */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Respawn")
	float _deathFXDelay;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Respawn")
	TSubclassOf<AActor> DeathFXToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Respawn")
	TSubclassOf<APawn> ThirdPersonCharacterToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Respawn")
	FVector spawnPosition;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Animation")
	bool actualyCrouch;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Menu")
	bool isInPause = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Menu")
	FTransform spawnPositionProjectile;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	virtual void Tick(float DeltaSeconds) override;

	void TryToPickAndDrop();
	void Pick();
	void Drop();

	void TryCrouch();

	void StartStafe();
	void EndStrafe();

	void Pause();

	void ShootPaintBall();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

