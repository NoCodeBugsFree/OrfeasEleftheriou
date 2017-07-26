// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "EQSEnvQueryGenerator_Cone.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API UEQSEnvQueryGenerator_Cone : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_BODY()
	
public:
	
	/** Generates items in a cone and places them in the environment */
	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	/* The distance between each point of the same Angle */
	UPROPERTY(EditAnywhere, Category = ConeProperties)
	float PointsDistance = 50.f;

	/* The maximum degrees of the generated cone */
	UPROPERTY(EditAnywhere, Category = ConeProperties)
	float ConeDegrees = 20.f;

	/* Angle Step is the step that the angles increase. A small value means that more item will get generated */
	UPROPERTY(EditAnywhere, Category = ConeProperties)
	float AngleStep;

	/* The radius of our cone */
	UPROPERTY(EditAnywhere, Category = ConeProperties)
	float ConeRadius = 150.f;
	
};
