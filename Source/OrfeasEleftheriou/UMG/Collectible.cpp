// Fill out your copyright notice in the Description page of Project Settings.

#include "Collectible.h"
#include "UMG/UMG_Character.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
ACollectible::ACollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating our components
	SM = CreateDefaultSubobject<UStaticMeshComponent>(FName("SM"));
	SetRootComponent(SM);
	SM->SetCollisionProfileName(FName("OverlapAll"));

	RotatingComp = CreateDefaultSubobject<URotatingMovementComponent>(FName("RotatingMoveComp"));
}

// Called when the game starts or when spawned
void ACollectible::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ACollectible::Overlap);
}

// Called every frame
void ACollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectible::Overlap(AActor* OverlappedActor, AActor* OtherActor)
{
	AUMG_Character* UMG_Character = Cast<AUMG_Character>(OtherActor);
	if (UMG_Character)
	{
		//Get the name of our item
		FText Text = FText::FromString(GetName());
		//Add the item to our UI
		UMG_Character->AddItemToUI(Text);
		Destroy();
	}
}

