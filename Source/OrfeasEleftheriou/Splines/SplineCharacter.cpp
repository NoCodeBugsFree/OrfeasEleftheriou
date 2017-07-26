// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineCharacter.h"
#include "Components/TimelineComponent.h"
#include "Splines/FlightStopActor.h"

ASplineCharacter::ASplineCharacter()
{
	FlightBoxCollider = CreateDefaultSubobject<UBoxComponent>(FName("FlightBoxCollider"));
	FlightBoxCollider->SetBoxExtent(FVector(150.f));
	FlightBoxCollider->SetupAttachment(GetRootComponent());
}

void ASplineCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// If the timeline has started, advance it by DeltaSeconds
	if (FlightTimeline.IsPlaying())
	{
		FlightTimeline.TickTimeline(DeltaSeconds);
	}
}

void ASplineCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Register a function that gets called when the box overlaps with a component
	FlightBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASplineCharacter::OnFlightBoxColliderOverlap);
}

void ASplineCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind the functions that execute on key press
	PlayerInputComponent->BindAction("NextFlightPath", IE_Pressed, this, &ASplineCharacter::NextFlightPathSelected);
	PlayerInputComponent->BindAction("PreviousFlightPath", IE_Pressed, this, &ASplineCharacter::PreviousFlightPathSelected);
}

void ASplineCharacter::TickTimeline(float Value)
{
	float SplineLength = ActiveSplineComponent->GetSplineLength();

	// Get the new location based on the provided values from the timeline.
	// The reason we're multiplying Value with SplineLength is because all our designed curves in the UE4 editor have a time range of 0 - X.
	// Where X is the total flight time.
	FVector NewLocation = ActiveSplineComponent->GetLocationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);

	SetActorLocation(NewLocation);

	FRotator NewRotation = ActiveSplineComponent->GetRotationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);

	// We're not interested in the pitch value of the above rotation so we make sure to set it to zero
	NewRotation.Pitch = 0;

	SetActorRotation(NewRotation);
}

void ASplineCharacter::OnFlightBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AFlightStopActor>())
	{
		// Store a reference of the nearby flight stop actor
		ActiveFlightStopActor = Cast<AFlightStopActor>(OtherActor);
	}
}

void ASplineCharacter::NextFlightPathSelected()
{
	if (ActiveFlightStopActor)
	{
		//Get the next flight path's spline component and update the flight timeline with the corresponding curve
		ActiveSplineComponent = ActiveFlightStopActor->GetNextFlightSplineComp();
		UpdateFlightTimeline(ActiveFlightStopActor->GetNextFlightCurve());
	}
}

void ASplineCharacter::PreviousFlightPathSelected()
{
	if (ActiveFlightStopActor)
	{
		//Get the previous flight path's spline component and update the flight timeline with the corresponding curve
		ActiveSplineComponent = ActiveFlightStopActor->GetPreviousFlightSplineComp();
		UpdateFlightTimeline(ActiveFlightStopActor->GetPreviousFlightCurve());
	}
}

void ASplineCharacter::UpdateFlightTimeline(UCurveFloat* CurveFloatToBind)
{
	//Initialize a timeline
	FlightTimeline = FTimeline();

	FOnTimelineFloat ProgressFunction;

	//Bind the function that ticks the timeline
	ProgressFunction.BindUFunction(this, FName("TickTimeline"));

	//Assign the provided curve and progress function for our timeline
	FlightTimeline.AddInterpFloat(CurveFloatToBind, ProgressFunction);
	FlightTimeline.SetLooping(false);
	FlightTimeline.PlayFromStart();

	//Set the timeline's length to match the last key frame based on the given curve
	FlightTimeline.SetTimelineLengthMode(TL_LastKeyFrame);

	//The ResetActiveFlightStopActor executes when the timeline finishes.
	//By calling ResetActiveFlightStopActor at the end of the timeline we make sure to reset any invalid references on ActiveFlightStopActor
	FOnTimelineEvent TimelineEvent;
	TimelineEvent.BindUFunction(this, FName("ResetActiveFlightStopActor"));
	FlightTimeline.SetTimelineFinishedFunc(TimelineEvent);
}

void ASplineCharacter::ResetActiveFlightStopActor()
{
	ActiveFlightStopActor = nullptr;
}

