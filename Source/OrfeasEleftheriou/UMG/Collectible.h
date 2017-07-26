// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectible.generated.h"

UCLASS()
class ORFEASELEFTHERIOU_API ACollectible : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ACollectible();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Static Mesh component */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SM;

	/* Just a simple but fancy rotation */
	UPROPERTY(VisibleAnywhere)
	class URotatingMovementComponent* RotatingComp;

	/* The function that fires when we have an overlap */
	UFUNCTION()
	void Overlap(AActor* OverlappedActor, AActor* OtherActor);
	
};
