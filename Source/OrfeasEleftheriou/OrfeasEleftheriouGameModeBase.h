// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrfeasEleftheriouGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API AOrfeasEleftheriouGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AOrfeasEleftheriouGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:



private:
	
};
