// Fill out your copyright notice in the Description page of Project Settings.

#include "ConstructionActor.h"


// Sets default values
AConstructionActor::AConstructionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create a Static Mesh for our Actor
	SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(SM);
}

// Called when the game starts or when spawned
void AConstructionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// NewObject<UStaticMeshComponent>
void AConstructionActor::OnConstruction(const FTransform& Transform)
{	
	Super::OnConstruction(Transform);

	//Empty the array and delete all it's components
	for (auto It = SMArray.CreateIterator(); It; It++)
	{
		(*It)->DestroyComponent();
	}

	SMArray.Empty();

	//Register all the components
	RegisterAllComponents();

	//The base name for all our components
	FName InitialName = FName("MyCompName");

	for (int32 i = 0; i < NumToSpawn; i++)
	{
		//Create a new Component
		//The first parameter is the "parent" of the our new component
		UStaticMeshComponent* NewComp = NewObject<UStaticMeshComponent>(this, InitialName);

		//Add a reference to our array
		SMArray.Add(NewComp);

		//Change the name for the next possible item
		FString Str = "MyCompName" + FString::FromInt(i + 1);

		//Convert the FString to FName
		InitialName = (*Str);

		//If the component is valid, set it's static mesh, relative location and attach it to our parent
		if (NewComp)
		{
			UE_LOG(LogTemp, Error, TEXT("Registering comp..."));

			//Register the new component
			NewComp->RegisterComponent();

			//Set the static mesh of our component
			NewComp->SetStaticMesh(SM->GetStaticMesh());


			//Set a random location based on the values we enter through the editor
			FVector Location;
			//Set the random seed in case we need to change our random values
			FMath::SRandInit(RandomSeed);

			Location.X += FMath::RandRange(-FMath::Abs(XThreshold), FMath::Abs(XThreshold));
			Location.Y += FMath::RandRange(-FMath::Abs(YThreshold), FMath::Abs(YThreshold));

			NewComp->SetWorldLocation(Location);

			//Attach the component to the root component
			NewComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

// Called every frame
void AConstructionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

