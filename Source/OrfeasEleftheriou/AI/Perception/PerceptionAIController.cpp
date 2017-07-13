// Fill out your copyright notice in the Description page of Project Settings.

#include "PerceptionAIController.h"
#include "AI/Perception/PerceptionAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

APerceptionAIController::APerceptionAIController()
{
	//Initialize the behavior tree and blackboard components
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void APerceptionAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	//Get the possessed Pawn. If it's the AI Character we created
	//initialize it's blackboard and start it's corresponding behavior tree
	APerceptionAICharacter* AICharacter = Cast<APerceptionAICharacter>(Pawn);
	if (AICharacter)
	{
		if (AICharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
			BehaviorComp->StartTree(*AICharacter->BehaviorTree);
		}
	}
}

void APerceptionAIController::SetSeenTarget(APawn* Pawn)
{
	//Registers the Pawn that the AI has seen in the blackboard
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(BlackboardKey, Pawn);
	}
}
