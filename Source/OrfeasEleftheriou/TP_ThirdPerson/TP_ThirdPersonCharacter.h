// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyDialogStructs.h"
#include "TP_ThirdPersonCharacter.generated.h"

UCLASS(config=Game)
class ORFEASELEFTHERIOU_API  ATP_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region ThirdPersonCharacter

	//
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:

	ATP_ThirdPersonCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

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

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
#pragma endregion

#pragma region Dialog

public:

	/* Generates the player lines */
	void GeneratePlayerLines(class UDataTable& PlayerLines);

	/*This array is essentially an Array of Excerpts from our dialogs!*/
	UPROPERTY(BlueprintReadOnly)
		TArray<FString> Questions;

	/*Performs the actual talking - informs the associated pawn if necessary in order to answer
	The subtitles array contain all the subtitles for this talk - it should be passed to our UI*/
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Talk(FString Excerpt, TArray<struct FSubtitle>& Subtitles);

	/*Enables / disables our talk ability. The player can't talk if he's not in a valid range*/
	void SetTalkRangeStatus(bool Status) { bIsInTalkRange = Status; }

	/*Sets a new associated pawn*/
	void SetAssociatedPawn(class ADialogAICharacter* Pawn) { AssociatedPawn = Pawn; }

	/*Retrieves the UI reference*/
	class UDialogUI* GetUI() { return UI; }

protected:

	/*The component responsible for playing our SFX*/
	UPROPERTY(VisibleAnywhere)
		class UAudioComponent* AudioComp;

	/*Opens or closes the conversation UI*/
	UFUNCTION(BlueprintImplementableEvent, Category = "AAA")
		void ToggleUI();

	/*UI Reference*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UDialogUI* UI;

private:

	/*True if the player is currently talking with any pawn*/
	bool bIsTalking;

	/*True if the player is inside a valid range to start talking to a pawn*/
	bool bIsInTalkRange;

	/*Initiates or terminates a conversation*/
	void ToggleTalking();

	/*The pawn that the player is currently talking to*/
	UPROPERTY()
		class ADialogAICharacter* AssociatedPawn;

	/*A reference to our lines - retrieved from the associated pawn*/
	class UDataTable* AvailableLines;

	/*Searches in the given row inside the specified table*/
	FDialog* RetrieveDialog(UDataTable* TableToSearch, FName RowName);


#pragma endregion

#pragma region AIHearing

public:

	/* The function that is going to play the sound and report it to our game */
	UFUNCTION(BlueprintCallable, Category = AI)
	void ReportNoise();

	/* A Pawn Noise Emitter component which is used in order to emit the sounds to nearby AIs */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPawnNoiseEmitterComponent* PawnNoiseEmitterComp;

protected:



private:

	/** Volume multiplier  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Volume = 1.f;

	/** Footstep sound  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class USoundBase* FootstepSoundToPlay;

#pragma endregion


#pragma region Reflection

public:

	/* This function will search which function to execute */
	UFUNCTION(BlueprintCallable, Category = Reflection)
	void ExecuteFunction(FString FunctionToExecute);

	UFUNCTION()
	void DoSomething();

#pragma endregion

};

