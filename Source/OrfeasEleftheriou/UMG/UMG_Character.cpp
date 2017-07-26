// Fill out your copyright notice in the Description page of Project Settings.

#include "UMG_Character.h"
#include "UMG/UMGPlayerController.h"

void AUMG_Character::BeginPlay()
{
	Super::BeginPlay();
}

void AUMG_Character::AddItemToUI(FText ItemName)
{
	AUMGPlayerController* UMGPlayerController = Cast<AUMGPlayerController>(GetController());

	if (UMGPlayerController)
	{
		UMGPlayerController->UpdateCollectedItems(ItemName);
	}
}
