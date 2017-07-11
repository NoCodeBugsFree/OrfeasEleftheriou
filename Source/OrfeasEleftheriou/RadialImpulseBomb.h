// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadialImpulseBomb.generated.h"

UCLASS()
class ORFEASELEFTHERIOU_API ARadialImpulseBomb : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ARadialImpulseBomb();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/** fill the array of overlapping actors  */
	void FillTheArrayOfOverlappedActors();

	/** AddRadialImpulse for each overlapping actor simulating physics  */
	void AddRadialImpulseToOverlappedActors();

	/** Add the force per tick  */
	void AddForce();

	/** Add Impulse for each overlapping actor simulating physics  */
	void AddImpulse();

	/**  SweepMultiByChannel example */
	void PerformSweep();

	/**  SweepMultiByChannel example */
	void PerformRaycast();

	/** Calls by timer  */
	void DoPeriodicCheck();
	
private:
	
	/* Box Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;
	
	/* Radius of the impulse*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Radius = 5000.f;

	/* Strength of the impulse */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Strength = 1000.f;

	/* A dynamic array of nearby actors */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> NearbyActors;

	/*The Force that we're going to apply to the chosen Actor with a default value*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	FVector ForceToAdd = FVector(0, 0, 5000);


	/* The Height of my Sphere starting from the location of the Actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float SphereHeight = 200;

	/* The Radius of the sphere trace */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float SphereRadius = 500;

	/* Sphere segments - used for visualization only */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 Segments = 100;

	/* Loop Time for timer handle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float LoopTime = 1.f;



	
};
