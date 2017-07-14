// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorWithOrfeasInteraface.h"


// Sets default values
AActorWithOrfeasInteraface::AActorWithOrfeasInteraface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AActorWithOrfeasInteraface::DoOrfeasInterfaceFunction()
{
	UE_LOG(LogTemp, Error, TEXT("DoOrfeasInterfaceFunction was called. . ."));
}

// Called when the game starts or when spawned
void AActorWithOrfeasInteraface::BeginPlay()
{
	Super::BeginPlay();

	/** Interface call  */
	DoOrfeasInterfaceFunction();
	
}

// Called every frame
void AActorWithOrfeasInteraface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

