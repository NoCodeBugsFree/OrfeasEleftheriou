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

	/* Executes right when the controller possess a Pawn */
	virtual void Possess(APawn* Pawn) override;

	/* Sets the sensed target in the blackboard */
	void SetSeenTarget(APawn* Pawn);

private:	
	
	/*Behavior Tree component reference*/
	UPROPERTY()
	class UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY()
	/*Blackboard comp ref*/
	class UBlackboardComponent* BlackboardComp;
	
};
