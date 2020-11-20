// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_CPPCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine.h"
#include "EngineGlobals.h"
#include "PaintBullet.h"

#include "Unreal_CPPGameMode.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AUnreal_CPPCharacter

AUnreal_CPPCharacter::AUnreal_CPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	GrabLocation = CreateDefaultSubobject<USceneComponent>(TEXT("GrabLocation"));
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	movement = GetCharacterMovement();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnreal_CPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("PickAndDrop", IE_Pressed, this, &AUnreal_CPPCharacter::TryToPickAndDrop);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AUnreal_CPPCharacter::TryCrouch);

	PlayerInputComponent->BindAction("Strafe", IE_Pressed, this, &AUnreal_CPPCharacter::StartStafe);
	PlayerInputComponent->BindAction("Strafe", IE_Released, this, &AUnreal_CPPCharacter::EndStrafe);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AUnreal_CPPCharacter::Pause);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUnreal_CPPCharacter::ShootPaintBall);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUnreal_CPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnreal_CPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUnreal_CPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUnreal_CPPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUnreal_CPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUnreal_CPPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUnreal_CPPCharacter::OnResetVR);
}


void AUnreal_CPPCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUnreal_CPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AUnreal_CPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AUnreal_CPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnreal_CPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUnreal_CPPCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

	float MoveForwardValue = Value;
}

void AUnreal_CPPCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AUnreal_CPPCharacter::Destroyed()
{
	const FVector Location = GetActorLocation();
	const FRotator Rotation = GetActorRotation();
	AActor* FireEffect = GetWorld()->SpawnActor<AActor>(DeathFXToSpawn, Location, Rotation);

	(*FireEffect).SetLifeSpan(_deathFXDelay);

	FRotator rot(0, 0, 0);

	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	if (AUnreal_CPPGameMode* GameMode = Cast<AUnreal_CPPGameMode>(GM)) {
		GameMode->CreateNewThirdCharacter(GetController(), spawnPosition, rot);
	}

	Super::Destroyed();
}

void AUnreal_CPPCharacter::TryToPickAndDrop()
{
	UE_LOG(LogTemp, Warning, TEXT("TryToPickAndDrop"));

	if (isHoldingObject)
	{
		Drop();
	}
	else
	{
		Pick();
	}

	
}

void AUnreal_CPPCharacter::Pick()
{
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * pickupDistance + GetActorLocation());

	FCollisionQueryParams TraceParams;

	GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ECC_PhysicsBody, TraceParams);

	if (Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Success !"));

		PhysicsHandle->GrabComponentAtLocation(Hit.Component.Get(), Hit.BoneName, Hit.Location);
		isHoldingObject = true;
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);
}

void AUnreal_CPPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector grabVector(200, 80, 0);
	const FTransform grabLocation(grabVector);

	GrabLocation->SetRelativeTransform(grabLocation);

	PhysicsHandle->SetTargetLocation(GrabLocation->GetComponentLocation());
}

void AUnreal_CPPCharacter::Drop()
{
	PhysicsHandle->ReleaseComponent();
	isHoldingObject = false;
}

void AUnreal_CPPCharacter::TryCrouch()
{
	if (actualyCrouch)
	{
		UnCrouch();
		actualyCrouch = false;
	}
	else
	{
		Crouch();
		actualyCrouch = true;
	}
}

void AUnreal_CPPCharacter::StartStafe()
{
	isStrafing = true;
	movement->bUseControllerDesiredRotation = true;
	movement->bOrientRotationToMovement = false;
	UE_LOG(LogTemp, Warning, TEXT("I am strafing"));
}

void AUnreal_CPPCharacter::EndStrafe()
{
	isStrafing = false;
	movement->bOrientRotationToMovement = true;
	UE_LOG(LogTemp, Warning, TEXT("I am not strafing"));
}

void AUnreal_CPPCharacter::Pause()
{
	if (isInPause)
	{
		isInPause = false;
	}
	else 
	{
		isInPause = true;
	}
}

void AUnreal_CPPCharacter::ShootPaintBall()
{
	if (!isHoldingObject)
	{
		FActorSpawnParameters parameters;
		FTransform tmpPos = GetActorTransform();
		tmpPos.SetLocation(GetActorRotation().Vector() * 200.0f + GetActorLocation());

		AActor* projectile = GetWorld()->SpawnActor<AActor>(APaintBullet::StaticClass(), tmpPos, parameters);
	}
}