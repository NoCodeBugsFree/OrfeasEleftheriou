// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FP_FirstPerson/FP_FirstPersonCharacter.h"
#include "UMG_Character.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API AUMG_Character : public AFP_FirstPersonCharacter
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	/* Adds the items to the collectibles UI */
	void AddItemToUI(FText ItemName);
	
};
