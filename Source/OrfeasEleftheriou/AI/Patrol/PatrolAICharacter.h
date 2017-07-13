// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolAICharacter.generated.h"

UCLASS()
class ORFEASELEFTHERIOU_API APatrolAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	APatrolAICharacter();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	class UBehaviorTree* BehaviorTree;
	
	
};
