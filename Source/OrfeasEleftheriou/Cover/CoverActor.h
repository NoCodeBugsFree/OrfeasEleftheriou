// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverActor.generated.h"

UCLASS()
class ORFEASELEFTHERIOU_API ACoverActor : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SM;

	/** Retrieves the movement direction and the facing rotation of the player */
	void RetrieveMovementDirectionAndFacingRotation(FVector& MovementDirection, FRotator& FacingRotation);
	
protected:

	// Sets default values for this actor's properties
	ACoverActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/** The box component that informs the player if he's able to take cover or not */
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComp;

private:

	UFUNCTION()
	void OnCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCompEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Returns true if the socket is close to the player */
	bool IsCloseToPlayer(FName SocketName);

	/** Determines the movement direction and the facing direction of the player */
	void DetermineMovementDirection(FVector& MovementDirection, FRotator& FacingDirection);

	/** Returns the name of the nearby socket */
	FName GetNearbySocket();
	
};
