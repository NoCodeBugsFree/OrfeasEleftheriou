// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FP_FirstPersonCharacter.generated.h"

// AsyncWork.h: Definition of queued work classes

class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USoundBase;
class UAnimMontage;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WT_Knife UMETA(DisplayName = "Knife"),
	WT_HandGun UMETA(DisplayName = "HandGun"),
	WT_Rifle UMETA(DisplayName = "Rifle")
};

UCLASS(config=Game)
class AFP_FirstPersonCharacter : public ACharacter
{

#pragma region Action Bindings With Parameters

public:



protected:

	/* The parameter which we will pass into the OneParamFunction */
	UPROPERTY(EditDefaultsOnly, Category = InputsWithParameters)
	int32 FirstIntParam = 50;

	/* The int parameter that will get passed into the TwoParamsFunction */
	UPROPERTY(EditDefaultsOnly, Category = InputsWithParameters)
	int32 SecondIntParam = 12345;

	/* The float parameter that will get passed into the TwoParamsFunction */
	UPROPERTY(EditDefaultsOnly, Category = InputsWithParameters)
	float FloatParam = 123.400002;


private:

	/* A simple function which prints its parameter */
	UFUNCTION()
	void OneParamFunction(int32 Param);

	/* A simple function which prints its parameters */
	UFUNCTION()
	void TwoParamsFunction(int32 IntParam, float FloatParameter);

	/* A function that changes the our parameters and replaces a hardcoded action bind */
	void ChangeParameters();

#pragma endregion


#pragma region Template FPS_Character

	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:

	AFP_FirstPersonCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/* This is when calculating the trace to determine what the weapon has hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponRange;

	/* This is multiplied by the direction vector when the weapon trace hits something to apply velocity to the component that is hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponDamage;

protected:

	/** Handler for a touch input beginning. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Fires a virtual projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

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

	/*
	* Performs a trace between two points
	*
	* @param	StartTrace	Trace starting point
	* @param	EndTrac		Trace end point
	* @returns FHitResult returns a struct containing trace result - who/what the trace hit etc.
	*/
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	/** Structure that handles touch data so we can process the various stages of touch. */
	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};

	/*
	* Handle begin touch event.
	* Stores the index and location of the touch in a structure
	*
	* @param	FingerIndex	The touch index
	* @param	Location	Location of the touch
	*/
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);

	/*
	* Handle end touch event.
	* If there was no movement processed this will fire a projectile, otherwise this will reset pressed flag in the touch structure
	*
	* @param	FingerIndex	The touch index
	* @param	Location	Location of the touch
	*/
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);

	/*
	* Handle touch update.
	* This will update the look position based on the change in touching position
	*
	* @param	FingerIndex	The touch index
	* @param	Location	Location of the touch
	*/
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);

	// Structure to handle touch updating
	TouchData	TouchItem;

	/*
	* Configures input for touchscreen devices if there is a valid touch interface for doing so
	*
	* @param	InputComponent	The input component pointer to bind controls to
	* @returns true if touch controls were enabled.
	*/
	void TryEnableTouchscreenMovement(UInputComponent* InputComponent);

public:

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


#pragma endregion

public:
	
	/* This function handles the weapon equipment */
	void EquipWeapon(EWeaponType EWeaponToEquip);

	/* Templated version of EquipWeapon which calls the above function */
	template<EWeaponType EWeaponToEquip>
	void EquipWeapon() { EquipWeapon(EWeaponToEquip); }
	
protected:
	
#pragma region MultiThreading

	/* Calculates prime numbers in the game thread */
	UFUNCTION(BlueprintCallable, Category = MultiThreading)
	void CalculatePrimeNumbers();

	/* Calculates prime numbers in a background thread */
	UFUNCTION(BlueprintCallable, Category = MultiThreading)
	void CalculatePrimeNumbersAsync();

	/* The max prime number */
	UPROPERTY(EditAnywhere, Category = MultiThreading)
	int32 MaxPrime = 500000;


	
private:

};

namespace ThreadingTest
{
	static void CalculatePrimeNumbers(int32 UpperLimit)
	{
		//Calculating the prime numbers...
		for (int32 i = 1; i <= UpperLimit; i++)
		{
			bool isPrime = true;

			for (int32 j = 2; j <= i / 2; j++)
			{
				if (FMath::Fmod(i, j) == 0)
				{
					isPrime = false;
					break;
				}
			}

			if (isPrime) GLog->Log("Prime number #" + FString::FromInt(i) + ": " + FString::FromInt(i));
		}
	}
}

/* PrimeCalculateAsyncTask is the name of our task
FNonAbandonableTask is the name of the class I've located from the source code of the engine */
class PrimeCalculationAsyncTask : public FNonAbandonableTask
{
	int32 MaxPrime;

public:

	/* Default constructor */
	PrimeCalculationAsyncTask(int32 MaxPrime)
	{
		this->MaxPrime = MaxPrime;
	}

	/* This function is needed from the API of the engine.
	My guess is that it provides necessary information
	about the thread that we occupy and the progress of our task */
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(PrimeCalculationAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	/* This function is executed when we tell our task to execute */
	void DoWork()
	{
		ThreadingTest::CalculatePrimeNumbers(MaxPrime);

		GLog->Log("--------------------------------------------------------------------");
		GLog->Log("End of prime numbers calculation on background thread");
		GLog->Log("--------------------------------------------------------------------");
	}
};

#pragma endregion