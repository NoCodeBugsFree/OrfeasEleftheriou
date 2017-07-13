// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PatrolAIController.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API APatrolAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	APatrolAIController();

	/** Returns Blackboard Comp  */
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
	/** Returns  Bot Target Points array */
	FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return BotTargetPoints; }


protected:



private:
	
	/*Behavior tree comp ref*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorComp;

	/*----------Blackboard----------*/

	/*Blackboard comp ref*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComp;

	/*Blackboard keys*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LocationToGoKey = "LocationToGo";

	/*----------Blackboard----------*/

	/*I use the AActor class as the Template class so I can use the GetAllActorsOfClass function.
	However, in this collection I'm going to store all the Available ABotTargetPoint instances.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> BotTargetPoints;

	/*Posses is executed when the character we want to control is spawned.
	Inside this function, we initialize the blackboard and start the behavior tree*/
	virtual void Possess(APawn* Pawn) override;
	
};
