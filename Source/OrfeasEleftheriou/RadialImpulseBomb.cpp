// Fill out your copyright notice in the Description page of Project Settings.

#include "RadialImpulseBomb.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ARadialImpulseBomb::ARadialImpulseBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(200.f));
}

// Called when the game starts or when spawned
void ARadialImpulseBomb::BeginPlay()
{
	Super::BeginPlay();
	
	/** fill the array of overlapping actors  */
	FillTheArrayOfOverlappedActors();

	/** AddRadialImpulse for each overlapping actor simulating physics  */
	//AddRadialImpulseToOverlappedActors();

	/** The difference between the Force and Impulse is that Impulse is recommended for one time instant burst.   */
	//AddImpulse();

	/**  SweepMultiByChannel example */
	//PerformSweep();
	
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ARadialImpulseBomb::DoPeriodicCheck, LoopTime, true);

}

// Called every frame
void ARadialImpulseBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// AddForce();

	// PerformRaycast();
}

void ARadialImpulseBomb::AddRadialImpulseToOverlappedActors()
{
	for (AActor* TestActor : NearbyActors)
	{
		if (TestActor)
		{
			UStaticMeshComponent* SM = Cast<UStaticMeshComponent>(TestActor->GetRootComponent());

			/* If the component is valid, apply a radial impulse from the location of the "Bomb" Actor
			* having as Radius and Strength the values from the Editor.
			* The RIF_Linear impulse fall off parameter means that we want the impulse to fall off to zero when
			* the max distance is reached.
			* The last parameter means that we want to ignore the mass of each Static Mesh
			*/
			if (SM)
			{
				SM->AddRadialImpulse(GetActorLocation(), Radius, Strength, ERadialImpulseFalloff::RIF_Linear, true);
			}
		}
	}
}

void ARadialImpulseBomb::AddForce()
{
	NearbyActors.Empty();
	FillTheArrayOfOverlappedActors();

	for (AActor* TestActor : NearbyActors)
	{
		if (TestActor && TestActor->GetRootComponent()->IsSimulatingPhysics())
		{
			UStaticMeshComponent* SM = Cast<UStaticMeshComponent>(TestActor->GetRootComponent());
			if (SM)
			{
				/* When you want to apply a force you always need to multiply it's value by the
				mass of the object that the object is applied to. */
				SM->AddForce(ForceToAdd * SM->GetMass());
			}
		}
	}
}

void ARadialImpulseBomb::AddImpulse()
{
	for (AActor* TestActor : NearbyActors)
	{
		if (TestActor && TestActor->GetRootComponent()->IsSimulatingPhysics())
		{
			UStaticMeshComponent* SM = Cast<UStaticMeshComponent>(TestActor->GetRootComponent());
			if (SM)
			{
				SM->AddImpulse(ForceToAdd * SM->GetMass());
			}
		}
	}
}

void ARadialImpulseBomb::PerformSweep()
{
	/* TArray is the collection that contains all the HitResults */
	TArray<FHitResult> HitResults;

	/* The Start location of the sphere */
	FVector StartLocation = GetActorLocation();

	/* The End location of the sphere is equal to the default location of the actor plus the height we will enter from the editor
	To extend this functionality, you can replace the height variable with a FVector */
	FVector EndLocation = GetActorLocation();
	EndLocation.Z += SphereHeight;

	/* Search for static objects only */
	ECollisionChannel ECC = ECollisionChannel::ECC_WorldStatic;

	/* Declare the Collision Shape, assign a Sphere shape and set it's radius */
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(SphereRadius);

	/* Perform the actual raycast. This method returns true if there is at least 1 hit. */
	bool bHitSomething = GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::FQuat(), ECC, CollisionShape);

	/* If the raycast hit a number of objects, iterate through them and print their name in the console */
	if (bHitSomething)
	{
		for (FHitResult& Hit : HitResults)
		{
			if (Hit.GetActor())
			{
				UE_LOG(LogTemp, Error, TEXT("%s"), *Hit.GetActor()->GetName());
			}
		}
	}

	/* In order to draw the sphere of the first image, I will use the DrawDebugSphere function which resides in the DrawDebugHelpers.h
	This function needs the center of the sphere which in this case is provided by the following equation */
	FVector CenterOfSphere = ((EndLocation - StartLocation) / 2) + StartLocation;

	/*Draw the sphere in the viewport*/
	DrawDebugSphere(GetWorld(), CenterOfSphere, CollisionShape.GetSphereRadius(), Segments, FColor::Green, true);

}

void ARadialImpulseBomb::PerformRaycast()
{
	//Hit contains information about what the raycast hit.
	FHitResult Hit;

	//The length of the ray in units.
	//For more flexibility you can expose a public variable in the editor
	float RayLength = 2000.f;

	//The Origin of the raycast
	FVector StartLocation = GetActorLocation();

	//The EndLocation of the raycast
	FVector EndLocation = StartLocation + (GetActorForwardVector() * RayLength);

	//Collision parameters. The following syntax means that we don't want the trace to be complex
	FCollisionQueryParams CollisionParameters;
	
	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_WorldStatic, CollisionParameters);

	//DrawDebugLine is used in order to see the raycast we performed
	//The boolean parameter used here means that we want the lines to be persistent so we can see the actual raycast
	//The last parameter is the width of the lines.
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1, 0, 1.f);
	
	if (Hit.GetActor())
	{
		UE_LOG(LogTemp, Error, TEXT("Hitted actor: %s"), *Hit.GetActor()->GetName());
	}
}

void ARadialImpulseBomb::DoPeriodicCheck()
{
	UE_LOG(LogTemp, Error, TEXT("DoPeriodicCheck() "));
}

void ARadialImpulseBomb::FillTheArrayOfOverlappedActors()
{
	TArray<AActor*> CollectedActors;
	BoxComponent->GetOverlappingActors(CollectedActors);

	for (AActor* TestActor : CollectedActors)
	{
		if (TestActor)
		{
			NearbyActors.AddUnique(TestActor);
		}
	}
}



