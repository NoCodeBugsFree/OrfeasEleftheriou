// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "SaveCharacter.generated.h"

/**
 * 
 */

UCLASS()
class ORFEASELEFTHERIOU_API ASaveCharacter : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()

public:

	friend FArchive& operator<<(FArchive& Ar, ASaveCharacter& SaveCharacter);

	void SaveLoadData(FArchive& Ar, float& HealthToSaveOrLoad, int32& CurrentAmmoToSaveOrLoad, FVector& PlayerLocationToSaveOrLoad);

protected:

	UFUNCTION(BlueprintCallable, Category = SaveLoad)
	bool SaveData();

	UFUNCTION(BlueprintCallable, Category = SaveLoad)
	bool LoadData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere)
	FVector RandomLocation;

private:
	
	
	
};
