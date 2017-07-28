// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MyLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API AMyLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "AAA")
	void DoSomethingBlueprintCallable();
	
	void DoSomethingCpp();
};
