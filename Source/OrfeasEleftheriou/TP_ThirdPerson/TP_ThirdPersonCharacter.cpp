// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DialogAICharacter.h"

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
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
	
	bIsTalking = false;
	bIsInTalkRange = false;
	AssociatedPawn = nullptr;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATP_ThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATP_ThirdPersonCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATP_ThirdPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATP_ThirdPersonCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATP_ThirdPersonCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATP_ThirdPersonCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATP_ThirdPersonCharacter::OnResetVR);

	InputComponent->BindAction("Talk", IE_Pressed, this, &ATP_ThirdPersonCharacter::ToggleTalking);
}

void ATP_ThirdPersonCharacter::GeneratePlayerLines(UDataTable& PlayerLines)
{
	//Get all the row names of the table
	if (PlayerLines.IsValidLowLevel())
	{
		TArray<FName> PlayerOptions = PlayerLines.GetRowNames();
		//For each row name try to retrieve the contents of the table
		for (auto It : PlayerOptions)
		{
			//Retrieve the contents of the table
			FDialog* Dialog = RetrieveDialog(&PlayerLines, It);

			if (Dialog)
			{
				//We retrieved a valid row - populate the questions array with our excerpts
				Questions.Add(Dialog->QuestionExcerpt);
			}
		}

		//Make sure to create a reference of the available line for later use
		AvailableLines = &PlayerLines;
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ATP_ThirdPersonCharacter.cpp - PlayerLines == NULL"));
	}
}

void ATP_ThirdPersonCharacter::Talk(FString Excerpt, TArray<FSubtitle>& Subtitles)
{
	//Get all the row names based on our stored lines
	TArray<FName> PlayerOptions = AvailableLines->GetRowNames();

	for (auto It : PlayerOptions)
	{
		//Search inside the available lines table to find the pressed Excerpt from the UI
		FDialog* Dialog = RetrieveDialog(AvailableLines, It);

		if (Dialog && Dialog->QuestionExcerpt == Excerpt)
		{
			//We've found the pressed excerpt - assign our sfx to the audio comp and play it
			AudioComp->SetSound(Dialog->SFX);
			AudioComp->Play();

			//Update the corresponding subtitles
			Subtitles = Dialog->Subtitles;


			if (UI && AssociatedPawn && Dialog->bShouldAIAnswer)
			{
				//Calculate the total displayed time for our subtitles
				//When the subtitles end - the associated pawn will be able to talk to our character

				TArray<FSubtitle> SubtitlesToDisplay;

				float TotalSubsTime = 0.f;

				for (int32 i = 0; i < Subtitles.Num(); i++)
				{
					TotalSubsTime += Subtitles[i].AssociatedTime;
				}

				//Just a hardcoded value in order for the AI not to answer right after our subs.
				//It would be better if we expose that to our editor? Sure!
				TotalSubsTime += 1.f;

				//Tell the associated pawn to answer to our character after the specified time!
				AssociatedPawn->AnswerToCharacter(It, SubtitlesToDisplay, TotalSubsTime);

			}
			else if (!Dialog->bShouldAIAnswer) ToggleTalking();
			break;

		}
	}
}

void ATP_ThirdPersonCharacter::ToggleTalking()
{
	if (bIsInTalkRange)
	{
		//If we are in talk range handle the talk status and the UI
		bIsTalking = !bIsTalking;
		ToggleUI();
		if (bIsTalking && AssociatedPawn)
		{
			//The associated pawn is polite enough to face us when we talk to him!
			FVector Location = AssociatedPawn->GetActorLocation();
			FVector TargetLocation = GetActorLocation();

			AssociatedPawn->SetActorRotation((TargetLocation - Location).Rotation());
		}
	}
}

FDialog* ATP_ThirdPersonCharacter::RetrieveDialog(UDataTable* TableToSearch, FName RowName)
{
	if (!TableToSearch) return nullptr;

	//The table is valid - retrieve the given row if possible
	FString ContextString;
	return TableToSearch->FindRow<FDialog>(RowName, ContextString);
}

void ATP_ThirdPersonCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATP_ThirdPersonCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ATP_ThirdPersonCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ATP_ThirdPersonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATP_ThirdPersonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATP_ThirdPersonCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !bIsTalking)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATP_ThirdPersonCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && !bIsTalking)
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
