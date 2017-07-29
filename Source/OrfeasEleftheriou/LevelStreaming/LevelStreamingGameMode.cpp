// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelStreamingGameMode.h"


void ALevelStreamingGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelMovePoint::StaticClass(), OutActors);

	for (auto It : OutActors)
	{
		// Get the transform of each actor
		Transforms.Add(It->GetTransform());
	}
}

void ALevelStreamingGameMode::MoveMap(FString MapName, int32 Position)
{
	if (Transforms.IsValidIndex(Position))
	{
		ULevelStreaming* level = UGameplayStatics::GetStreamingLevel(GetWorld(), *MapName);

		// store the new map name and the new transform
		this->MapName = MapName;
		MapTransform = Transforms[Position];

		if (level->IsLevelVisible())
		{
			// If the level is visible we need to unload it first
			// and then spawn it in a new location

			// The unload stream level is essentially an async operation
			// Using the Latent Action Info we're able to create a callback
			// which is fired when the UnloadStreamLevel finishes its execution
			FLatentActionInfo info;
			info.CallbackTarget = this;
			info.ExecutionFunction = FName("SpawnMap");
			info.UUID = 0;
			info.Linkage = 0;

			UGameplayStatics::UnloadStreamLevel(GetWorld(), *MapName, info);
		}
		// If the level is not visible just spawn the map
		else SpawnMap();
	}
}

void ALevelStreamingGameMode::SpawnMap()
{
	FLatentActionInfo info;

	// We load the stream level but we make sure not to make it visible before we assign a transform to it
	// The reason I've typed *MapName is the parameters is because the second parameters needs an FName so
	// essentially I'm type casting the FString to FName
	UGameplayStatics::LoadStreamLevel(GetWorld(), *MapName, false, false, info);
	ULevelStreaming* level = UGameplayStatics::GetStreamingLevel(GetWorld(), *MapName);

	// Assign a new transform to our level
	level->LevelTransform = MapTransform;

	// Make the level visible
	level->bShouldBeVisible = true;
}
