// Fill out your copyright notice in the Description page of Project Settings.

#include "RadialImpulseBomb.h"
#include "Components/BoxComponent.h"

// Sets default values
ARadialImpulseBomb::ARadialImpulseBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(FVector(200.f));
}

// Called when the game starts or when spawned
void ARadialImpulseBomb::BeginPlay()
{
	Super::BeginPlay();
	
	/** fill the array of overlapping actors  */
	TArray<AActor*> CollectedActors;
	BoxComponent->GetOverlappingActors(CollectedActors);
	
	for (AActor* TestActor : CollectedActors)
	{
		if (TestActor)
		{
			NearbyActors.AddUnique(TestActor);
		}
	}
	
	/** AddRadialImpulse for each overlapping actor simulating physics  */
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

// Called every frame
void ARadialImpulseBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

