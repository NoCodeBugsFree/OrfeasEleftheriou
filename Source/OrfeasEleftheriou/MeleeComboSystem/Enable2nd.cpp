// Fill out your copyright notice in the Description page of Project Settings.

#include "Enable2nd.h"
#include "MyAnimInstance.h"


void UEnable2nd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UMyAnimInstance* MyAnimInstance = Cast<UMyAnimInstance>(MeshComp->GetAnimInstance());

	if (MyAnimInstance)
	{
		MyAnimInstance->SetEnableSecondAttackInput(true);
	}
}

