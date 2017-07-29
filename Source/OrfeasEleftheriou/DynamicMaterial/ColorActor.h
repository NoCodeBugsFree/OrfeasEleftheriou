// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ColorActor.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API AColorActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
};
