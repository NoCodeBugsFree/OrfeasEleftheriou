// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillsTreeCharacter.h"
#include "SkillTree/SkillsComponent.h"
#include "SkillTree/Skill.h"

ASkillsTreeCharacter::ASkillsTreeCharacter()
{
	// Create the root component for our spring arms
	SkillsRootComp = CreateDefaultSubobject<USceneComponent>(FName("SkillsRootComp"));

	// Attach it to our root
	SkillsRootComp->SetupAttachment(RootComponent);

	// Create the spring arm components and attach them to their root
	LevelOneSpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("LevelOneSpringArm"));
	LevelTwoSpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("LevelTwoSpringArm"));
	LevelThreeSpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("LevelThreeSpringArm"));
	
	LevelOneSpringArm->SetupAttachment(SkillsRootComp);
	LevelTwoSpringArm->SetupAttachment(SkillsRootComp);
	LevelThreeSpringArm->SetupAttachment(SkillsRootComp);

	// Initializing the skills component
	SkillsComponent = CreateDefaultSubobject<USkillsComponent>(FName("SkillsComponent"));
}

void ASkillsTreeCharacter::Fire(bool bShouldFireSecondary /*= false*/)
{
	// This is a dummy logic - we will only have 2 skills for this post
	TSubclassOf<ASkill> SkillBP = (bShouldFireSecondary && SkillsComponent->SkillsArray.IsValidIndex(1)) ?
		SkillsComponent->SkillsArray[1] : SkillsComponent->SkillsArray[0];

	if (SkillBP)
	{
		FActorSpawnParameters ActorSpawnParams;

		TArray<FTransform> SpawnTransforms = GetSpawnTransforms(SkillBP->GetDefaultObject<ASkill>()->GetLevel());

		for (int32 i = 0; i < SpawnTransforms.Num(); i++)
		{
			GetWorld()->SpawnActor<ASkill>(SkillBP, SpawnTransforms[i]);
			UE_LOG(LogTemp, Error, TEXT("Your message"));
		}
	}
}

FTransform ASkillsTreeCharacter::GetFixedSpringArmTransform(USpringArmComponent* SpringArm)
{
	FTransform Transform;
	if (SpringArm)
	{
		Transform = SpringArm->GetComponentTransform();
		// We want a fixed location for our transform, since we don't want to spawn our skills
		// right on top of our character.
		Transform.SetLocation(Transform.GetLocation() + SpringArm->GetForwardVector() * 100);
	}
	return Transform;
}

TArray<FTransform> ASkillsTreeCharacter::GetSpawnTransforms(int32 level)
{
	TArray<FTransform> SpawnPoints;
	switch (level)
	{
	case 1:
	{
		SpawnPoints.Add(GetFixedSpringArmTransform(LevelOneSpringArm));
		break;
	}
	case 2:
	{
		SpawnPoints.Add(GetFixedSpringArmTransform(LevelTwoSpringArm));
		SpawnPoints.Add(GetFixedSpringArmTransform(LevelThreeSpringArm));
		break;
	}
	case 3:
	{
		SpawnPoints.Add(GetFixedSpringArmTransform(LevelOneSpringArm));
		SpawnPoints.Add(GetFixedSpringArmTransform(LevelTwoSpringArm));
		SpawnPoints.Add(GetFixedSpringArmTransform(LevelThreeSpringArm));
	}
	default:
		break;
	}
	return SpawnPoints;
}
