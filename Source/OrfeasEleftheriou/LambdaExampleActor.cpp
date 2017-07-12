// Fill out your copyright notice in the Description page of Project Settings.

#include "LambdaExampleActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALambdaExampleActor::ALambdaExampleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALambdaExampleActor::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;

	FTimerDelegate TimerDelegate;

	//Binding our Lambda expression
	TimerDelegate.BindLambda([&]()
	{
		//Typing the logic of our function here just like any other function...
		UE_LOG(LogTemp, Error, TEXT("Destroying Actor now..."));
		Destroy();
	});//Don't forget the ";" in the end of your parenthesis!

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 5.f, false);
	
	

	// -------- Lambda with parameters ------------------------------------------

	//Declaring an Array of Actors
	TArray<AActor*> ActorsArray;

	//Declaring a delegate with one int32 parameter
	DECLARE_DELEGATE_OneParam(MyUsefulDelegate, int32);

	//The following functions populates the ActorsArray with all the Actors which reside inside our current level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsArray);

	//Declaring a MyUsefulDelegate
	MyUsefulDelegate Del;

	//Binding a Lambda to it
	//In this lambda we pass the ActorsArray by value since we won't make any changes
	//or want any changes reflected outside the lambda expression
	//If we don't pass the ActorsArray in the capture list then we won't be able to have access to it!
	Del.BindLambda([ActorsArray](int32 ActorIndex)
	{
		//Print the name of the Actor which has an index equal to the one we provided (ActorIndex)
		//Make sure we provided a valid index for our array
		if (ActorsArray.IsValidIndex(ActorIndex))	
		{
			UE_LOG(LogTemp, Error, TEXT("Actor with given index: %s"), *ActorsArray[ActorIndex]->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("You've entered an invalid index. That's unfortunate :("));
		}
			
	});

	//Show me the 16th Actor of the Array - Don't forget that TArray is zero-based!
	Del.ExecuteIfBound(15);
}

// Called every frame
void ALambdaExampleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

