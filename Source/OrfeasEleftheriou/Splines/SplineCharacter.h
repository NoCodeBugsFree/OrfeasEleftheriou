// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "SplineCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API ASplineCharacter : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()

public:

	ASplineCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/* The Box component that detects any nearby flight stops */
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* FlightBoxCollider;

private:

	/** The flight timeline */
	FTimeline FlightTimeline;

	/** The function that ticks the timeline */
	UFUNCTION()
	void TickTimeline(float Value);

	/** The active spline component, meaning the flight path that the character is currently following */
	UPROPERTY()
	USplineComponent* ActiveSplineComponent;

	/** The selected flight stop actor */
	UPROPERTY()
	class AFlightStopActor* ActiveFlightStopActor;

	/** Box overlap function */
	UFUNCTION()
	void OnFlightBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Executes when we're pressing the NextFlightPath key bind */
	void NextFlightPathSelected();

	/** Executes when we're pressing the PreviousFlightPath key bind */
	void PreviousFlightPathSelected();

	/** Updates the flight timeline with a new curve and starts the flight */
	void UpdateFlightTimeline(UCurveFloat* CurveFloatToBind);

	UFUNCTION()
	void ResetActiveFlightStopActor();

};
