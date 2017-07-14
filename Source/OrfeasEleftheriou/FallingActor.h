// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallingActor.generated.h"

UCLASS()
class ORFEASELEFTHERIOU_API AFallingActor : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AFallingActor();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/* Static Mesh Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SM;

	/* OnHit function is called when the FallingActor hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/**
	* Event when this actor bumps into a blocking object, or blocks another actor that bumps into it.
	* This could happen due to things like Character movement, using Set Location with 'sweep' enabled, or physics simulation.
	* For events when objects overlap (e.g. walking into a trigger) see the 'Overlap' event.
	*
	* @note For collisions during physics simulation to generate hit events, 'Simulation Generates Hit Events' must be enabled.
	* @note When receiving a hit from another object's movement (bSelfMoved is false), the directions of 'Hit.Normal' and 'Hit.ImpactNormal'
	* will be adjusted to indicate force from the other object against this object.
	*/
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
public:	
	

	
	
};
