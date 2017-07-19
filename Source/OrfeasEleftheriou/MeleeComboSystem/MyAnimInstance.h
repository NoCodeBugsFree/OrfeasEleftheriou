// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	/* Determines which attack animation will be played */
	void Attack();

	// NativeUpdateAnimation
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetEnableSecondAttackInput(bool NewSecondAttackInput) { bAcceptsSecondAttackInput = NewSecondAttackInput; }

	void SetEnableThirdAttackInput(bool NewThirdAttackInput) { bAcceptsThirdAttackInput = NewThirdAttackInput; }
	
protected:

	/* True means that we're currently in air - or falling */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFalling;

	/* Holds a reference to the character's movement speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MovementSpeed;

	/* The attack montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* MeleeAttackMontage;
	
	/* True if the character can perform a second attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAcceptsSecondAttackInput;

	/* True if the character can perform a third attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAcceptsThirdAttackInput;

private:
	
	
};
