 // Fill out your copyright notice in the Description page of Project Settings.

#include "Enable3rd.h"
#include "MyAnimInstance.h"


void UEnable3rd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UMyAnimInstance* MyAnimInstance = Cast<UMyAnimInstance>(MeshComp->GetAnimInstance());

	if (MyAnimInstance)
	{
		MyAnimInstance->SetEnableThirdAttackInput(true);
	}
}
