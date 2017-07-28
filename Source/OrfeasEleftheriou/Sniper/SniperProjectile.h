// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SniperProjectile.generated.h"

class ASniperEnemyCharacter;

UCLASS()
class ORFEASELEFTHERIOU_API ASniperProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ASniperProjectile();

	/** Applies the velocity multiplier to the existing velocity of the projectile */
	void ApplyVelocityMultiplier();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* The velocity multiplier that is applied when we slow the world's time */
	UPROPERTY(EditAnywhere)
	float VelocityMultiplier = 5.f;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ProjectileCameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* ProjectileSpringArmComp;

	/* The distance threshold that the death camera transition will occur */
	UPROPERTY(EditAnywhere)
	float DeathCameraTransitionDistance = 300.f;
	
private:

	// OnComponentBeginOverlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	/* Projectile Movement Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	 
	/*  Collision Sphere Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComp;
	
	/* Projectile Mesh Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ProjectileMesh;
	
	/* The enemy that this projectile is going to kill */
	UPROPERTY()
	ASniperEnemyCharacter* EnemyToKill;

	/* True when the transition from the projectile's camera to the enemy's camera has been activated */
	bool bActivatedTransition = false;

public:	
	
	/* Sets the enemy that is going to be killed */
	FORCEINLINE void SetEnemyToKill(ASniperEnemyCharacter* Enemy) { EnemyToKill = Enemy; }
	
};
