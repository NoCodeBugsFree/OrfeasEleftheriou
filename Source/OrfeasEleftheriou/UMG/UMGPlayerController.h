// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UMGPlayerController.generated.h"

/**
 * 
 */

class UCollectiblesUI;

UCLASS()
class ORFEASELEFTHERIOU_API AUMGPlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	/* Executes when the controller possess a pawn */
	virtual void Possess(APawn* InPawn) override;

	/* Updates the UI based on the item that the player has collected */
	void UpdateCollectedItems(FText ItemName);
	
protected:

	/* Property which is pointing to our Widget Blueprint in order to instantiate it using c++ */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCollectiblesUI> CollectiblesWidgetBP;

private:
	
	/* Our UI Reference */
	UPROPERTY()
	UCollectiblesUI* CollectiblesWidget;
	
};
