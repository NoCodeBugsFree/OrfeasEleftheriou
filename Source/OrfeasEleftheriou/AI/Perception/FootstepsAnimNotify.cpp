// Fill out your copyright notice in the Description page of Project Settings.

#include "FootstepsAnimNotify.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

void UFootstepsAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		ATP_ThirdPersonCharacter* TP_ThirdPersonCharacter = Cast<ATP_ThirdPersonCharacter>(MeshComp->GetOwner());
		if (TP_ThirdPersonCharacter)
		{
			TP_ThirdPersonCharacter->ReportNoise();
		}
	}
}
