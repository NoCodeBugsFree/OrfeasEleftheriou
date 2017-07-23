// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryPlayerController.h"
#include "InventoryWidget.h"
#include "FP_FirstPerson/FP_FirstPersonCharacter.h"

 void AInventoryPlayerController::HandleInventoryInput()
{
	 AFP_FirstPersonCharacter* FP_FirstPersonCharacter = Cast<AFP_FirstPersonCharacter>(GetPawn());
	 if (FP_FirstPersonCharacter)
	 {
		 if (InventoryWidgetRef)
		 {
			 if (bIsInventoryOpen)
			 {
				 // Mark the inventory as closed
				 bIsInventoryOpen = false;

				 // Remove it from the viewport
				 InventoryWidgetRef->RemoveFromViewport();

				 // Hide the cursor of our game
				 bShowMouseCursor = false;

				 //Tell the game that we want to register inputs for our game only and not for our UI
				 FInputModeGameOnly InputMode;
				 SetInputMode(InputMode);

				 // Unpause the game
				 SetPause(false);
			 }
			 else
			 {
				 // Mark the inventory as open
				 bIsInventoryOpen = true;

				 // Re-populate the ItemsArray
				 InventoryWidgetRef->ItemsArray = FP_FirstPersonCharacter->GetInventory();

				 // Show the inventory
				 InventoryWidgetRef->Show();

				 // Show the cursor of our game
				 bShowMouseCursor = true;

				 // Tell the game that we want to register inputs both for our game and UI
				 FInputModeGameAndUI InputMode;
				 SetInputMode(InputMode);
				 
				 // Pause the game
				 SetPause(true);
			 }
		 }
	 }
}

 void AInventoryPlayerController::Possess(APawn* InPawn)
 {
	 Super::Possess(InPawn);

	 if (InventoryWidgetBP)
	 {
		 // Create the Inventory Widget based on the Blueprint reference we will input from within the Editor
		 InventoryWidgetRef = CreateWidget<UInventoryWidget>(this, InventoryWidgetBP);
	 }
	 else
	 {
		 UE_LOG(LogTemp, Error, TEXT("InventoryWidgetBP == NULL [AInventoryPlayerController] "));
	 }
 }
