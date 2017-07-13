// Fill out your copyright notice in the Description page of Project Settings.

#include "DelegateActor.h"


// Sets default values
ADelegateActor::ADelegateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADelegateActor::BeginPlay()
{
	Super::BeginPlay();

	//Declaring a delegate of MyDelegate type
	MyDelegate MyDel;

	//Binding a UFUNCTION to MyDel - this will not call the function just yet.
	MyDel.BindUFunction(this, FName("SomeFunction"));

	//Calling the bound function of the delegate
	MyDel.Execute();

	//Declaring a delegate of MyIntDelegate type
	MyIntDelegate IntDelegate;

	//Binding two UFUNCTIONs to IntDelegate - this will not call any function just yet
	IntDelegate.BindUFunction(this, FName("IntFunction"));
	IntDelegate.BindUFunction(this, FName("SecondIntFunction"));

	//Since the IntDelegate is a single-cast delegate it will only contain up to one function
	//In this case, the IntDelegate contains the last bound function
	IntDelegate.Execute(50);

	//Declaring a delegate of MyIntMulticastDelegate type
	MyIntMulticastDelegate Del;

	//Adding three UFUNCTIONs to the delegate - this will not call any function just yet
	Del.AddUFunction(this, FName("IntFunction"));
	Del.AddUFunction(this, FName("SecondIntFunction"));
	Del.AddUFunction(this, FName("ThirdIntFunction"));

	//Calling all the bound functions with a value of 10
	Del.Broadcast(10);
	
}

// Called every frame
void ADelegateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADelegateActor::IntFunction(int32 x)
{
	UE_LOG(LogTemp, Error, TEXT("Output from IntFunction: %s"), *FString::FromInt(x));
}

void ADelegateActor::SecondIntFunction(int32 x)
{
	UE_LOG(LogTemp, Error, TEXT("Output from SecondIntFunction: %s"), *FString::FromInt(x * 2));
}

void ADelegateActor::ThirdIntFunction(int32 x)
{
	//x to square
	float power = FMath::Pow(x, 2);
	UE_LOG(LogTemp, Error, TEXT("Third Int Function: %s"), *FString::SanitizeFloat(power));
}

void ADelegateActor::SomeFunction()
{	
	UE_LOG(LogTemp, Error, TEXT("Some function log"));
}

