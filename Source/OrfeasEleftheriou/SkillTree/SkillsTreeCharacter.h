// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "SkillsTreeCharacter.generated.h"

/**
 * 
 */

class ASkill;
class USkillsComponent;

UCLASS()
class ORFEASELEFTHERIOU_API ASkillsTreeCharacter : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "AAA")
	USkillsComponent* GetSkillsComponent() { return SkillsComponent; };

protected:

	ASkillsTreeCharacter();

	/* The root component in which the spring arm components will be attached */
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SkillsRootComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* LevelOneSpringArm;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* LevelTwoSpringArm;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* LevelThreeSpringArm;

	/* Skills Component reference */
	UPROPERTY(VisibleAnywhere/*, meta = (AllowPrivateAccess = "true")*/)
	USkillsComponent* SkillsComponent;

	/* Fires a skill */
	UFUNCTION(BlueprintCallable, Category = TLSkillsTree)
	void Fire(bool bShouldFireSecondary = false);

private:
	
	/* Returns a fixed transform based on the given spring arm comp */
	FTransform GetFixedSpringArmTransform(USpringArmComponent* SpringArm);

	/* Returns an array of transform in order to determine how many skills will get spawned */
	TArray<FTransform> GetSpawnTransforms(int32 level);
	
};
