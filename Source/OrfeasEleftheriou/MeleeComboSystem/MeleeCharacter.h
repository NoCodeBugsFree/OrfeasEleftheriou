// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "MeleeCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API AMeleeCharacter : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()
	
public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void Attack();

private:
	
	
};

