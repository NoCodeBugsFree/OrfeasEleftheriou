// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IOrfeasInterface.h"
#include "ActorWithOrfeasInteraface.generated.h"

UCLASS()
class ORFEASELEFTHERIOU_API AActorWithOrfeasInteraface : public AActor, public IOrfeasInterface
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AActorWithOrfeasInteraface();

	/** override interface function  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	virtual void DoOrfeasInterfaceFunction() override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	

	
	
};
