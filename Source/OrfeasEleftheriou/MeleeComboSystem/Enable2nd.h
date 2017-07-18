// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Enable2nd.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API UEnable2nd : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
