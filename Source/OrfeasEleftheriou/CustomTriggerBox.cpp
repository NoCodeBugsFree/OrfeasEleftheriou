// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerBox.h"
#include "Components/BoxComponent.h"

// Sets default values
ACustomTriggerBox::ACustomTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void ACustomTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACustomTriggerBox::OnTriggerEnter);
	OnActorEndOverlap.AddDynamic(this, &ACustomTriggerBox::OnTriggerExit);
}

// Called every frame
void ACustomTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACustomTriggerBox::OnTriggerEnter(AActor* OverlapedActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Error, TEXT("%s starts overlaps me"), *OtherActor->GetName());
	}
}

void ACustomTriggerBox::OnTriggerExit(AActor* OverlapedActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Error, TEXT("%s ends overlaps me"), *OtherActor->GetName());
	}
}
