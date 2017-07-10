// Fill out your copyright notice in the Description page of Project Settings.

#include "OrfeasEleftheriouGameModeBase.h"

AOrfeasEleftheriouGameModeBase::AOrfeasEleftheriouGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AOrfeasEleftheriouGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AOrfeasEleftheriouGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GLog->Log("Hello from Tick!");
}