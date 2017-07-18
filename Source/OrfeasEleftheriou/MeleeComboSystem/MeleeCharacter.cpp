// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeCharacter.h"
#include "MyAnimInstance.h"

void AMeleeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("PerformMeleeHit", IE_Pressed, this, &AMeleeCharacter::Attack);
}

void AMeleeCharacter::Attack()
{
	UMyAnimInstance* MyAnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnimInstance)
	{
		MyAnimInstance->Attack();
	}
}
