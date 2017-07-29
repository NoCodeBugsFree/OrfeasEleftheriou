// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorActor.h"

void AColorActor::BeginPlay()
{
	Super::BeginPlay();

	// Get the first material of the static mesh and turn it into a material instance
	UMaterialInstanceDynamic* DynamicMatInstance = GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0);

	// Generate random RGB and Alpha values for our Vector Parameter
	FLinearColor RandomColor = FLinearColor::MakeRandomColor();
	
	// If we have a valid dynamic material instance, modify its parameters
	if (DynamicMatInstance)
	{
		DynamicMatInstance->SetVectorParameterValue(FName("ColorParam"), RandomColor);
		DynamicMatInstance->SetScalarParameterValue(FName("MetalParam"), FMath::RandRange(0, 1));
	}
}
