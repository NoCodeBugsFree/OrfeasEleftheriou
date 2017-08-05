// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveCharacter.h"

// Before we implement the logic for these functions, 
// define a name for the file we’re going to store into your characters source file:
#define SAVEDATAFILENAME "SampleSavedData"


FArchive& operator<<(FArchive& Ar, ASaveCharacter& SaveCharacter)
{
	// Ar << SaveCharacter;
	Ar << SaveCharacter.Health;

	return Ar;
}

void ASaveCharacter::SaveLoadData(FArchive& Ar, float& HealthToSaveOrLoad, int32& CurrentAmmoToSaveOrLoad, FVector& PlayerLocationToSaveOrLoad)
{
	/**
			The reason that SaveLoadData function can do both is because the “<<”
			operator has two different meanings. Specifically, the operator in this
			case could mean that: We’re storing data from a variable (ie HealthToSaveOrLoad)
			into a binary format We’re converting binary data to a variable
	 */

	// Save or load values
	Ar << HealthToSaveOrLoad;

	Ar << CurrentAmmoToSaveOrLoad;

	Ar << PlayerLocationToSaveOrLoad;
}

bool ASaveCharacter::SaveData()
{
	// Save the data to binary
	FBufferArchive ToBinary;
	SaveLoadData(ToBinary, Health, CurrentAmmo, RandomLocation);

	// No data were saved
	if (ToBinary.Num() <= 0) return false;

	// Save binaries to disk
	bool result = FFileHelper::SaveArrayToFile(ToBinary, TEXT(SAVEDATAFILENAME));

	// Empty the buffer's contents
	ToBinary.FlushCache(); // Flushes cache and frees internal data.
	ToBinary.Empty();      // Empties the array. 

	return result;
}

bool ASaveCharacter::LoadData()
{
	TArray<uint8> BinaryArray;

	// load disk data to binary array
	if ( !FFileHelper::LoadFileToArray(BinaryArray, TEXT(SAVEDATAFILENAME)) )
	{
		return false;
	}

	if (BinaryArray.Num() <= 0)
	{
		return false;
	}

	// Memory reader is the archive that we're going to use in order to read the loaded data
	FMemoryReader FromBinary = FMemoryReader(BinaryArray, true); //  Archive for reading arbitrary data from the specified memory location
	FromBinary.Seek(0);

	SaveLoadData(FromBinary, Health, CurrentAmmo, RandomLocation);

	// Empty the buffer's contents
	FromBinary.FlushCache(); // Flushes cache and frees internal data.
	BinaryArray.Empty();
	// Close the stream
	FromBinary.Close();

	return true;
}


// -----------------------------------------------------------------------------------

/** 
	Saving and Loading complex types

	we saved primitive types 
	that already exist in the engine's API. Fortunately, 
	its quite easy to save and load complex types 
	(for example custom classes). To do that,
	we have to extend the << operator of the FArchive class, 
	in order to support our custom class.

	Navigate to your class header file and declare
	the following friend function:
	friend FArchive& operator<<(FArchive& Ar, CustomClass& CustomClassRef);
	Then, in your source file provide the following implementation:
*/


//friend FArchive& operator<<(FArchive& Ar, ASaveCharacter& SaveCharacter)
//{
//	Ar << SaveCharacter.Health;
//
//	return Ar;
//}
