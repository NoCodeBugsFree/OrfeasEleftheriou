// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelStreamingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API ALevelStreamingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	/** Moves or spawns a map in the BaseMap.
	* @param MapName - the name of the map we want to move
	* @param Position - the index of the Transforms array we want to use
	*/
	UFUNCTION(Exec, Category = LevelStreaming)
	void MoveMap(FString MapName, int32 Position);

	virtual void BeginPlay() override;

protected:



private:
	
	/** Contains all the available transforms of LevelMovePointActors. */
	TArray<FTransform> Transforms;

	/** Map name to spawn */
	FString MapName;

	/** The desired transform of our map */
	FTransform MapTransform;

	/** Spawns the desired map based on the MapName and the MapTransform */
	UFUNCTION()
	void SpawnMap();

};
