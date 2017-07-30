// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FP_FirstPerson/FP_FirstPersonHUD.h"
#include "RadarHUD.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API ARadarHUD : public AFP_FirstPersonHUD
{
	GENERATED_BODY()

public:

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

protected:

	/**
	 * So, let’s say that my resolution is 1920 x 1080. If I multiply both
	 * the 1920 and the 1080 with a value of 0 I would get the upper left
	 * corner of the screen. With that said, here is a graph explaining how this works:
	 * 
	 *   -------- H U D -----------
	 *    x = 0              x = 1
	 *    -------------------------
	 *	  |						  | y = 0 
	 *	  |						  |
	 *	  |			H U D		  |
	 *	  |						  | 
	 *    ------------------------- y = 1
	 *
	 *  The X and Y values, correspond to the values of our multiplier 
	 * (in this case: RadarStartLocation). So, if I enter the values 
	 * 0.9 and 0.2 on the radar start location, UE4 will place 
	 * our radar somewhere close to the top right corner. 
	 */ 

	/* The start location of our radar */
	UPROPERTY(EditAnywhere, Category = Radar)
	FVector2D RadarStartLocation = FVector2D(0.9f, 0.2f);

	/* The radius of our radar */
	UPROPERTY(EditAnywhere, Category = Radar)
	float RadarRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = Radar)
	float DegreeStep = 0.25f;

	/* The pixel size of the drawable radar actors */
	UPROPERTY(EditAnywhere, Category = Radar)
	float DrawPixelSize = 5.f;
	
	/* Sphere height and radius for our raycast */
	UPROPERTY(EditAnywhere, Category = Radar)
	float SphereHeight = 200.f;

	UPROPERTY(EditAnywhere, Category = Radar)
	float SphereRadius = 2750.f;

	/* Holds a reference to every actor we are currently drawing in our radar */
	UPROPERTY()
	TArray<AActor*> RadarActors;
	
	/* The distance scale of the radar actors */
	UPROPERTY(EditAnywhere, Category = Radar)
	float RadarDistanceScale = 25.f;

	/* Converts the given actors' location to local (based on our character) */
	FVector2D ConvertWorldLocationToLocal(AActor* ActorToPlace);

	/* Draws the raycasted actors in our radar */
	void DrawRaycastedActors();

private:
	
	/* Returns the center of the radar as a 2d vector */
	FVector2D GetRadarCenterPosition();

	/* Draws the radar */
	void DrawRadar();

	/** Draws the player */
	void DrawPlayerInRadar();

	/** Sphere Trace for search actor to draw on radar  */
	void PerformRadarRaycast();
	
};
