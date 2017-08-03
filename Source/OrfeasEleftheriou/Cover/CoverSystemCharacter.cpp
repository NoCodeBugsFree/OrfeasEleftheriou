// Fill out your copyright notice in the Description page of Project Settings.

#include "CoverSystemCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Cover/CoverActor.h"

// Sets default values
ACoverSystemCharacter::ACoverSystemCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
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
		
	// bCanCrouch = true
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 150.f;
	GetCharacterMovement()->CrouchedHalfHeight = 45.f;
}

// Called when the game starts or when spawned
void ACoverSystemCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoverSystemCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACoverSystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	/** Jump  */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	/** Move Forward/Backward  */
	PlayerInputComponent->BindAxis("MoveForward", this, &ACoverSystemCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACoverSystemCharacter::MoveRight);
	
	/** Pitch/Yaw  */
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	/** Toggle Crouch  */
	InputComponent->BindAction("Crouch", IE_Pressed, this, &ACoverSystemCharacter::ToggleCrouch);

	/**  Take Cover */
	InputComponent->BindAction("TakeCover", IE_Pressed, this, &ACoverSystemCharacter::ToggleCover);
}

void ACoverSystemCharacter::MoveForward(float Value)
{
	//We don't want to move forward or backwards when we're taking cover
	if ((Controller != NULL) && (Value != 0.0f) && !bIsInCover)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACoverSystemCharacter::MoveRight(float Value)
{
	// for the animation bp
	CoverValue = Value;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (!bIsInCover)
		{
			// default movement functionality

			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
		}
		else
		{
			// Move according to the cover actor's position
			AddMovementInput(CoverDirectionMovement, Value);
		}
	}
}

void ACoverSystemCharacter::ToggleCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		// The player is already crouching - disable the crouch effect
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ACoverSystemCharacter::ToggleCover()
{
	if (GetCharacterMovement()->IsCrouching() && bCanTakeCover)
	{
		bIsInCover = !bIsInCover;

		if (bIsInCover && Cover)
		{
			// This is done because my downloaded animations do not require an orientation to movement
			// Depending on your animation you may (or not) need this
			GetCharacterMovement()->bOrientRotationToMovement = false;

			FRotator CoverRotation;
			Cover->RetrieveMovementDirectionAndFacingRotation(CoverDirectionMovement, CoverRotation);
			SetActorRotation(CoverRotation);
		}
		else
		{
			// This is done because my downloaded animations do not require an orientation to movement
			// Depending on your animation you may (or not) need this
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
	}
}

void ACoverSystemCharacter::SetCanTakeCover(bool CanTakeCover, ACoverActor* CoverActor)
{
	if (!CanTakeCover && bIsInCover)
	{
		ToggleCover();
	}

	bCanTakeCover = CanTakeCover;
	Cover = CoverActor;
}

