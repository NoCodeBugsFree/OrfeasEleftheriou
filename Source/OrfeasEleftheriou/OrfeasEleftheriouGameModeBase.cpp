// Fill out your copyright notice in the Description page of Project Settings.

#include "OrfeasEleftheriouGameModeBase.h"

AOrfeasEleftheriouGameModeBase::AOrfeasEleftheriouGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AOrfeasEleftheriouGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	DataTableHandling();
}

void AOrfeasEleftheriouGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AOrfeasEleftheriouGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	Super::EndPlay(EndPlayReason);
}

void AOrfeasEleftheriouGameModeBase::DoSomething()
{
	UE_LOG(LogTemp, Error, TEXT("DoSomething was called . . . "));
}

void AOrfeasEleftheriouGameModeBase::DataTableHandling()
{
	// Retrieve the Row based on the Row Name
	// The first parameter is the row name
	// I have yet to determine the second one, however, I will open a ticket on ue4 answerhub
	FPlayerStats* Row = StatsDB->FindRow<FPlayerStats>(TEXT("2"), TEXT(""));
	if (Row)
	{
		//Cast the XpToLevel to string so I can print it with GLog
		FString output = FString::FromInt((*Row).XpToLevel);
		GLog->Log(output);

		//Casting the Health to string...
		output = FString::SanitizeFloat((*Row).Health);
		GLog->Log(output);

		/** simple set one variable from this Data table  */
		OnePlayerStat.XpToLevel = Row->XpToLevel;
		OnePlayerStat.Health = Row->Health;

		/** read all datatable data to array  */
		FString ContextString;
		TArray<FName> RowNames;
		RowNames = StatsDB->GetRowNames();

		for (auto& name : RowNames)
		{
			FPlayerStats* RowFromArray = StatsDB->FindRow<FPlayerStats>(name, ContextString);
			if (RowFromArray)
			{
				ArrayOfPlayerStats.Add(*RowFromArray);
			}
		}

		/** loop through DataTable   */
		for (auto it : StatsDB->RowMap) /** TMap<FName, uint8*> RowMap; // Map of name of row to row data structure. */
		{
			// it.Key has the key from first column of the CSV file
			// UE_LOG(LogTemp, Error, TEXT("it.Key = %s"), *it.Key.ToString());
			// it.Value has a pointer to a struct of data. You can safely cast it to your actual type, e.g FMyStruct* data = (FMyStruct*)(it.Value);
			FPlayerStats* RowFromLoopArray = (FPlayerStats*)(it.Value);
			if (RowFromLoopArray)
			{
				LoopArrayOfPlayerStats.Add(*RowFromLoopArray);
			}
		}
	}
}

