// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FP_FirstPerson/FP_FirstPersonCharacter.h"
#include "SniperCharacter.generated.h"

/**
 * 
 */

class ASniperProjectile;
class ASniperEnemyCharacter;

UCLASS()
class ORFEASELEFTHERIOU_API ASniperCharacter : public AFP_FirstPersonCharacter
{
	GENERATED_BODY()
	
public:

	ASniperCharacter();

	virtual void BeginPlay() override;

	// SetupPlayerInputComponent
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Enables the FirstPersonCamera again */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ResetActiveCamera();

protected:

	/* The time dilation multiplier */
	UPROPERTY(EditAnywhere, Category = "AAA")
	float TimeDilationMultiplier = 0.05f;

	/** Dummy sniper fire shot  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void OnSniperFire();
	
	/**
	 * Calls to activate projectile camera in  
	 * ThirdPersonCameraToProjectileCameraBlendTime delay 
	 */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ActivateProjectileCameraInDelay(ASniperProjectile* SpawnedProjectile, UWorld* const World);
	
	/** /Plays the fire shot sound and animation montage  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void PlayFireFX();

	/** Sniper projectile type  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA")
	TSubclassOf<class ASniperProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* ThirdPersonSpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ThirdPersonCameraComp;
	
	/* The time in seconds that we will transition from the third person camera to the projectile's camera*/
	UPROPERTY(EditAnywhere)
	float ThirdPersonCameraToProjectileCameraBlendTime = 0.05f;

	/* The delay in seconds that we will activate the projectile's camera*/
	UPROPERTY(EditAnywhere)
	float ThirdPersonToProjectileTransitionDelay = 0.005f;

	/* The raycast length */
	UPROPERTY(EditAnywhere)
	float RaycastLength = 2000.f;

private:
	
	/* De-activates the default camera and activates the third person camera */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ActivateThirdPersonCamera();

	/* De-activates the third person camera and activates the camera on the given projectile*/
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ActivateProjectileCamera(ASniperProjectile* Projectile);

	/* Returns true if the projectile hits an enemy - false otherwise */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool HitsAnEnemy(ASniperProjectile* Projectile, ASniperEnemyCharacter*& HitEnemy);
	/** 
		It's a reference to a pointer to an ASniperEnemyCharacter.
		This means the function in question can modify the pointer 
		as well as the ASniperEnemyCharacter itself.

		You can just pass a pointer in, 
		the one complication being that the pointer needs to be an l-value,
		not just an r-value
	*/

	/* Performs a raycast and returns the hit actor - if any */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	AActor* GetHitActor(ASniperProjectile* Projectile);

public:

	/** Returns ThirdPersonCameraComp subobject **/
	FORCEINLINE class UCameraComponent* GetThirdPersonCameraComponent() const { return ThirdPersonCameraComp; }

};
