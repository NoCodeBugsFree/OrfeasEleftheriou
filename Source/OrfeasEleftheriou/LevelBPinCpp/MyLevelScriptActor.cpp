// Fill out your copyright notice in the Description page of Project Settings.

#include "MyLevelScriptActor.h"

void AMyLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	DoSomethingCpp();
}

void AMyLevelScriptActor::DoSomethingBlueprintCallable()
{
	UE_LOG(LogTemp, Error, TEXT("DoSomethingBlueprintCallable"));
}

void AMyLevelScriptActor::DoSomethingCpp()
{
	UE_LOG(LogTemp, Error, TEXT("DoSomethingCpp"));
}
