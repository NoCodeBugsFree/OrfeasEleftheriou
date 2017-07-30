// Fill out your copyright notice in the Description page of Project Settings.

#include "RadarHUD.h"

void ARadarHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawRadar();

	DrawPlayerInRadar();

	PerformRadarRaycast();

	DrawRaycastedActors();

	// Empty the radar actors in case the player moves out of range,
	// by doing so, we have always a valid display in our radar
	RadarActors.Empty();
}

FVector2D ARadarHUD::ConvertWorldLocationToLocal(AActor* ActorToPlace)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Player && ActorToPlace)
	{
		// Convert the world location to local, based on the transform of the player
		FVector ActorsLocal3dVector = Player->GetTransform().InverseTransformPosition(ActorToPlace->GetActorLocation());

		// Rotate the vector by 90 degrees counter-clockwise in order to have a valid rotation in our radar
		ActorsLocal3dVector = FRotator(0.f, -90.f, 0.f).RotateVector(ActorsLocal3dVector);

		// Apply the given distance scale
		ActorsLocal3dVector /= RadarDistanceScale;

		// Return a 2d vector based on the 3d vector we've created above
		return FVector2D(ActorsLocal3dVector);
	}
	return FVector2D(0, 0);
}

void ARadarHUD::DrawRaycastedActors()
{
	FVector2D RadarCenter = GetRadarCenterPosition();

	for (auto It : RadarActors)
	{
		FVector2D convertedLocation = ConvertWorldLocationToLocal(It);

		// We want to clamp the location of our actors in order to make sure
		// that we display them inside our radar

		// To do so, I've created the following temporary vector in order to access
		// the GetClampedToMaxSize2d function. This functions returns a clamped vector (if needed)
		// to match our max length
		FVector tempVector = FVector(convertedLocation.X, convertedLocation.Y, 0.f);

		// Subtract the pixel size in order to make the radar display more accurate
		tempVector = tempVector.GetClampedToMaxSize2D(RadarRadius - DrawPixelSize);

		// Assign the converted X and Y values to the vector we want to display
		convertedLocation.X = tempVector.X;
		convertedLocation.Y = tempVector.Y;

		DrawRect(FLinearColor::Red, RadarCenter.X + convertedLocation.X, RadarCenter.Y + convertedLocation.Y, DrawPixelSize, DrawPixelSize);
	}
}

FVector2D ARadarHUD::GetRadarCenterPosition()
{
	// If the canvas is valid, return the center as a 2d vector
	return (Canvas) ? FVector2D(Canvas->SizeX*RadarStartLocation.X, Canvas->SizeY*RadarStartLocation.Y) : FVector2D(0, 0);
}

void ARadarHUD::DrawRadar()
{
	FVector2D RadarCenter = GetRadarCenterPosition();

	for (float i = 0; i < 360; i += DegreeStep)
	{
		// We want to draw a circle in order to represent our radar
		// In order to do so, we calculate the sin and cos of almost every degree
		// It it impossible to calculate each and every possible degree because they are infinite
		// Lower the degree step in case you need a more accurate circle representation

		// We multiply our coordinates by radar size 
		// in order to draw a circle with radius equal to the one we will input through the editor
		float fixedX = FMath::Cos(i) * RadarRadius;
		float fixedY = FMath::Sin(i) * RadarRadius;

		// Actual draw
		DrawLine(RadarCenter.X, RadarCenter.Y, RadarCenter.X + fixedX, RadarCenter.Y + fixedY, FLinearColor::Gray, 1.f);
	}
}

void ARadarHUD::DrawPlayerInRadar()
{
	FVector2D RadarCenter = GetRadarCenterPosition();

	DrawRect(FLinearColor::Blue, RadarCenter.X, RadarCenter.Y, DrawPixelSize, DrawPixelSize);
}

void ARadarHUD::PerformRadarRaycast()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Player)
	{
		TArray<FHitResult> HitResults;
		FVector EndLocation = Player->GetActorLocation();
		EndLocation.Z += SphereHeight;

		FCollisionShape CollisionShape;
		CollisionShape.ShapeType = ECollisionShape::Sphere;
		CollisionShape.SetSphere(SphereRadius);

		// Perform a the necessary sweep for actors.
		GetWorld()->SweepMultiByChannel(HitResults, Player->GetActorLocation(), EndLocation, FQuat::Identity,
			ECollisionChannel::ECC_WorldDynamic, CollisionShape);

		for (auto It : HitResults)
		{
			AActor* CurrentActor = It.GetActor();
			//In case the actor contains the word "Radar" as a tag, add it to our array
			if (CurrentActor && CurrentActor->ActorHasTag("Radar"))
			{
				RadarActors.Add(CurrentActor);
			}
		}
	}
}
