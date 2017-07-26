// Fill out your copyright notice in the Description page of Project Settings.

#include "UMGPlayerController.h"
#include "UMG/CollectiblesUI.h"
#include "UMG/UMG_Character.h"

void AUMGPlayerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	
	if (CollectiblesWidgetBP)
	{
		// Creating our widget and adding it to our viewport
		CollectiblesWidget = CreateWidget<UCollectiblesUI>(this, CollectiblesWidgetBP);

		CollectiblesWidget->AddToViewport();
	}
}

void AUMGPlayerController::UpdateCollectedItems(FText ItemName)
{
 	AUMG_Character* UMG_Character = Cast<AUMG_Character>(GetPawn());
 
 	if (UMG_Character)
 	{
 		// Updating our UI with the new collected item
 		CollectiblesWidget->AddItemToUI(ItemName);
 	}
}
