// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyHelperFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ORFEASELEFTHERIOU_API UMyHelperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "AAA")
	static void UsefulFunction();

protected:



private:
	
	
};
