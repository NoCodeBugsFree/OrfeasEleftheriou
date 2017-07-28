#pragma once
#include "Engine.h"
#include "MyDialogStructs.generated.h"

/** Subtitle struct  */
USTRUCT(BlueprintType)
struct FSubtitle : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/*The subtitle that will be displayed for a specific period of time in our UI*/
	UPROPERTY(EditAnywhere)
	FString Subtitle;

	/*The relative time in seconds, that the subtitle will appear*/
	UPROPERTY(EditAnywhere)
	float AssociatedTime;
};

/** Dialog struct  */
USTRUCT(BlueprintType)
struct FDialog : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/*The question's excerpt - this will be shown inside our UI*/
	UPROPERTY(EditAnywhere)
	FString QuestionExcerpt;

	/*The actual SFX that we're going to play*/
	UPROPERTY(EditAnywhere)
	USoundBase* SFX;

	/*An array of the associated subtitles*/
	UPROPERTY(EditAnywhere)
	TArray<FSubtitle> Subtitles;

	/*True if we want to wait for the AI to respond*/
	UPROPERTY(EditAnywhere)
	bool bShouldAIAnswer;
};