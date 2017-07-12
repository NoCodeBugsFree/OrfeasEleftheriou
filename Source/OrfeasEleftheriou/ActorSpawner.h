// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorSpawner.generated.h"

class AUsefulActor;

UCLASS()
class ORFEASELEFTHERIOU_API AActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorSpawner();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/*Blueprint Reference of UsefulActor class*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AUsefulActor> UsefulActorBP;

	/*Delay after the Blueprint of the UsefulActor will get spawned*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float TimeToSpawn = 2.f;

	/*Spawns the UsefulActor Blueprint*/
	UFUNCTION()
	void SpawnUsefulActor();

public:	
	

	
	
};
