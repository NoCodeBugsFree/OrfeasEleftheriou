// Fill out your copyright notice in the Description page of Project Settings.

#include "PerceptionAICharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "AI/Perception/PerceptionAIController.h"

// Sets default values
APerceptionAICharacter::APerceptionAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing the pawn sensing component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	//Set the peripheral vision angle to 90 degrees
	PawnSensingComp->SetPeripheralVisionAngle(90.f);
}

// Called when the game starts or when spawned
void APerceptionAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Register the function that is going to fire when the character sees a Pawn
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &APerceptionAICharacter::OnSeePlayer);
	}
}

// Called every frame
void APerceptionAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APerceptionAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APerceptionAICharacter::OnSeePlayer(APawn* Pawn)
{
	APerceptionAIController* AIController = Cast<APerceptionAIController>(GetController());
	//Set the seen target on the blackboard
	if (AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("APerceptionAICharacter::OnSeePlayer"));
		AIController->SetSeenTarget(Pawn);
	}
}

