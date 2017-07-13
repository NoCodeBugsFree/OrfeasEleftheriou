// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingActor.h"


// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Initializing the static mesh of the actor
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();

	//Checking if the curvefloat that was entered in the blueprint was valid
	if (CurveFloat)
	{
		/*The ProgressFunction contains the signature of the function that is
		going to execute every time we tick our timeline.
		Think of the progressfunction as a delegate!*/
		FOnTimelineFloat ProgressFunction;

		/*In order to bind the function our ufunction we need to create a FName which contains
		the name of the function we want to call every time the timeline advances.
		Binding the HandleProgress function to the ProgressFunction...*/
		ProgressFunction.BindUFunction(this, FName("HandleProgress"));

		//Setting up the loop status and the function that is going to fire when the timeline ticks
		MyTimeline.AddInterpFloat(CurveFloat, ProgressFunction);
		MyTimeline.SetLooping(true);

		/*Initializing our home and target location in order to make the actor float between them.
		As you can see the two locations have the same X and Y but their Z value is different.*/
		ActorInitialLocation = TargetLocation = GetActorLocation();
		TargetLocation.Z += MaxBounceHeight;

		//Starting the timeline...
		MyTimeline.PlayFromStart();
	}
}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Advancing the timeline in every tick
	MyTimeline.TickTimeline(DeltaTime);
}

void AFloatingActor::HandleProgress(float Value)
{
	//Setting up the new location of our actor
	FVector NewLocation = FMath::Lerp(ActorInitialLocation, TargetLocation, Value);
	SetActorLocation(NewLocation);
}