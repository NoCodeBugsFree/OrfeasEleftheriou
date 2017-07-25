// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PerceptionAICharacter.generated.h"

UCLASS()
class ORFEASELEFTHERIOU_API APerceptionAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	APerceptionAICharacter();

	/* The Component which is used for the "seeing" sense of the AI */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	/* The Behavior Tree of the Character */
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	/** Calls every time when this character see the player  */
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	/* Hearing function - will be executed when we hear a Pawn */
	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);
	
};
