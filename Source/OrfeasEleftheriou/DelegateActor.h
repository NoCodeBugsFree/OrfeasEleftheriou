// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DelegateActor.generated.h"

UCLASS()
class ORFEASELEFTHERIOU_API ADelegateActor : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ADelegateActor();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Single-cast delegate declaration. No parameters */
	DECLARE_DELEGATE(MyDelegate)

	/* Single-cast delegate declaration. One int32 parameter */
	DECLARE_DELEGATE_OneParam(MyIntDelegate, int32)

	/* Multi-cast delegate declaration. One int32 parameter */
	DECLARE_MULTICAST_DELEGATE_OneParam(MyIntMulticastDelegate, int32)

	/* Function with one int32 parameter */
	UFUNCTION()
	void IntFunction(int32 x);

	/* Function with one int32 parameter */
	UFUNCTION()
	void SecondIntFunction(int32 x);

	/* Function with one int32 parameter */
	UFUNCTION()
	void ThirdIntFunction(int32 x);

	/* Function with no parameters */
	UFUNCTION()
	void SomeFunction();

};
