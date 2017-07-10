// Fill out your copyright notice in the Description page of Project Settings.

#include "OrfeasEleftheriouGameModeBase.h"

AOrfeasEleftheriouGameModeBase::AOrfeasEleftheriouGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AOrfeasEleftheriouGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	PrintOnScreen(1);
}

void AOrfeasEleftheriouGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AOrfeasEleftheriouGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	Super::EndPlay(EndPlayReason);
}
