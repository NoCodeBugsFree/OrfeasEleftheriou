// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomTriggerBox.generated.h"

UCLASS()
class ORFEASELEFTHERIOU_API ACustomTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ACustomTriggerBox();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	UFUNCTION()
	void OnTriggerEnter(AActor* OverlapedActor, AActor* OtherActor);
	
	UFUNCTION()
	void OnTriggerExit(AActor* OverlapedActor, AActor* OtherActor);
	
	/* Box Collision Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;
	
public:	
	
};
