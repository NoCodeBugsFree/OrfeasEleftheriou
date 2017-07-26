// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PerceptionAIController.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API APerceptionAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	APerceptionAIController();

	/* Blackboard key */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName BlackboardKey = "Target";

	/* This property is used to find a certain key for our blackboard.
	We will create the blackboard later in this tutorial*/
	UPROPERTY(EditDefaultsOnly)
	FName TargetKey = "SensedPawn";
	
	/* Sets the sensed target in the blackboard */
	void SetSeenTarget(APawn* Pawn);
	
	/* Sets the new sensed target value inside our Blackboard values */
	void SetSensedTarget(APawn* NewTarget);

private:	

	/* Executes right when the controller possess a Pawn */
	virtual void Possess(APawn* Pawn) override;
	
	/* Behavior Tree component reference */
	UPROPERTY()
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY()
	/* Blackboard comp ref */
	class UBlackboardComponent* BlackboardComponent;
	
};
