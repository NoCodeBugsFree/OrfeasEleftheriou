// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlightStopActor.generated.h"

class USplineComponent;

UCLASS()
class ORFEASELEFTHERIOU_API AFlightStopActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	// Sets default values for this actor's properties
	AFlightStopActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/** Returns the previous flight curve */
	UCurveFloat* GetPreviousFlightCurve() { return PreviousFlightCurve; };

	/** Returns the next flight curve */
	UCurveFloat* GetNextFlightCurve() { return NextFlightCurve; };

	/** Returns the next flight spline component */
	USplineComponent* GetNextFlightSplineComp() { return NextFlightStop; };

	/**Returns the previous flight spline component */
	USplineComponent* GetPreviousFlightSplineComp() { return PreviousFlightStop; };

protected:
	
	/** The FloatCurve corresponding to the previous flight spline component */
	UPROPERTY(EditAnywhere)
	UCurveFloat* PreviousFlightCurve;

	/** The FloatCurve corresponding to the next flight spline component */
	UPROPERTY(EditAnywhere)
	UCurveFloat* NextFlightCurve;

	/** A static mesh for our flight stop */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SM;

	/** The spline component that describes the flight path of the next flight */
	UPROPERTY(VisibleAnywhere)
	USplineComponent* NextFlightStop;

	/** The spline component that describes the flight path of the previous flight */
	UPROPERTY(VisibleAnywhere)
	USplineComponent* PreviousFlightStop;
	
};
