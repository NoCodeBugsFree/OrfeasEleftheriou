// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoverSystemCharacter.generated.h"

class ACoverActor;

UCLASS()
class ORFEASELEFTHERIOU_API ACoverSystemCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	
	/** Inform the player that he's able to take cover in the provided actor */
	void SetCanTakeCover(bool bCanTakeCover, ACoverActor* CoverActor);

	/** Returns the input while in cover */
	UFUNCTION(BlueprintCallable, Category = CoverSystem)
	float CoverValueInput() const { return CoverValue; }

	/** Returns true if we're in cover*/
	UFUNCTION(BlueprintCallable, Category = CoverSystem)
	bool IsInCover() const { return bIsInCover; }

protected:

	// Sets default values for this character's properties
	ACoverSystemCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

private:

	/** Enables or disables the crouch mode */
	void ToggleCrouch();

	/** Enables or disables the cover mode */
	void ToggleCover();

	/** True if the player can take cover */
	bool bCanTakeCover = false;

	/** True if the player is currently taking cover */
	bool bIsInCover = false;

	/** The movement direction while taking cover */
	FVector CoverDirectionMovement;

	/** Cover reference*/
	UPROPERTY()
	ACoverActor* Cover;

	/** Contains the input of the cover */
	float CoverValue;

public:	
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};
