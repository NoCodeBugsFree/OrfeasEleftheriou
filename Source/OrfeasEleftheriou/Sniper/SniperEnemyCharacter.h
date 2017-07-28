// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "SniperEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API ASniperEnemyCharacter : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()
	
public:

	ASniperEnemyCharacter();

	/* Transitions the active camera to the corresponding death camera */
	void EnableCameraTransition();

	/* Kills the character and activates the FirstPerson camera again */
	void Die();

protected:

	/* The blend time in seconds, from the projectile's camera to corresponding death camera*/
	UPROPERTY(EditAnywhere)
	float DeathCameraBlendTime = 0.05f;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* DeathSpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* DeathCameraComp;

	/* The delay the camera will be reset right after the death of the enemy */
	UPROPERTY(EditAnywhere)
	float CameraResetDelay = 1.f;
	
};
