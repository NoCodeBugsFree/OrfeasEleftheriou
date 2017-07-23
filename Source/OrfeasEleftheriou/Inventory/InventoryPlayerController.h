// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API AInventoryPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/* Opens or closes the inventory */
	void HandleInventoryInput();

	virtual void Possess(APawn* InPawn) override;

protected:

	/* InventoryWidget Blueprint template */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInventoryWidget> InventoryWidgetBP;

private:

	/* InventoryWidget reference */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UInventoryWidget* InventoryWidgetRef;

	/* True if the inventory is currently open - false otherwise */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	bool bIsInventoryOpen = false;

};
