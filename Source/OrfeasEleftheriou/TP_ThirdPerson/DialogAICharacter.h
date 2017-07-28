// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyDialogStructs.h"
#include "DialogAICharacter.generated.h"

UCLASS()
class ORFEASELEFTHERIOU_API ADialogAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ADialogAICharacter();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// -----------------------------------------------------------------------------------

	/* If the player is inside this box component he will be able to initiate a conversation with the pawn */
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComp;

	/* The audio component responsible for playing the dialog coming from this pawn */
	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* AudioComp;

	/* The player lines - each pawn can offer different dialog options for our character */
	UPROPERTY(EditAnywhere, Category = "AAA")
	class UDataTable* PlayerLines;

	/* The AI lines */
	UPROPERTY(EditAnywhere, Category = "AAA")
	class UDataTable* AILines;

private:
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void Talk(class USoundBase* SFX, TArray<struct FSubtitle> Subs);
	
public:	
	
	/* Answers to the character after a specified delay */
	void AnswerToCharacter(FName PlayerLine, TArray<struct FSubtitle>& SubtitlesToDisplay, float delay);

	/* Retrieves the corresponding character lines */
	UDataTable* GetPlayerLines() { return PlayerLines; }

	
};
