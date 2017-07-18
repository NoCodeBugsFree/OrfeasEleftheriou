// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"

void UMyAnimInstance::Attack()
{
	if (MeleeAttackMontage)
	{
		FName CurrentSection = Montage_GetCurrentSection(MeleeAttackMontage);

		// Determine which section is currently playing and jump to the next section is possible
		
		if (CurrentSection.IsNone())
		{
			Montage_Play(MeleeAttackMontage);
		}
		else if (CurrentSection.IsEqual("FirstAttack") && bAcceptsSecondAttackInput)
		{
			Montage_JumpToSection(FName("SecondAttack"), MeleeAttackMontage);
		}
		else if (CurrentSection.IsEqual("SecondAttack") && bAcceptsThirdAttackInput)
		{
			Montage_JumpToSection(FName("ThirdAttack"), MeleeAttackMontage);
		}
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	/** Sets the MovementSpeed  */
	APawn* TestPawn = TryGetPawnOwner();
	if (TestPawn)
	{
		MovementSpeed = TestPawn->GetVelocity().Size();
	}

	/** Disable 2nd and 3rd attack animations if montage is not playing  */
	if (MeleeAttackMontage)
	{
		if ( !Montage_IsPlaying(MeleeAttackMontage) )
		{
			bAcceptsSecondAttackInput = false;
			bAcceptsThirdAttackInput  = false;
		}
	}
}
