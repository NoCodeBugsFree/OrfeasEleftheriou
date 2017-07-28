// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogAICharacter.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "TP_ThirdPersonCharacter.h"
#include "DialogUI.h"


// Sets default values
ADialogAICharacter::ADialogAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Init the box and audio comps
	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADialogAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Register the begin and end overlap functions
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADialogAICharacter::OnBoxOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ADialogAICharacter::OnBoxEndOverlap);
}

// Called every frame
void ADialogAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADialogAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADialogAICharacter::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && OtherActor->IsA<ATP_ThirdPersonCharacter>())
	{
		ATP_ThirdPersonCharacter* Char = Cast<ATP_ThirdPersonCharacter>(OtherActor);
		if (Char)
		{
			Char->SetTalkRangeStatus(true);
			Char->GeneratePlayerLines(*PlayerLines);
			Char->SetAssociatedPawn(this);
		}	
	}
}

void ADialogAICharacter::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA<ATP_ThirdPersonCharacter>())
	{
		ATP_ThirdPersonCharacter* Char = Cast<ATP_ThirdPersonCharacter>(OtherActor);
		if (Char)
		{
			Char->SetTalkRangeStatus(false);
			Char->SetAssociatedPawn(nullptr);
		}
	}
}

 void ADialogAICharacter::Talk(USoundBase* SFX, TArray<FSubtitle> Subs)
 {
	 ATP_ThirdPersonCharacter* Char = Cast<ATP_ThirdPersonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	 //Play the corresponding sfx
	 AudioComp->SetSound(SFX);
	 AudioComp->Play();

	 //Tell the UI to update with the new subtitles
	if (Char)
	{
		Char->GetUI()->UpdateSubtitles(Subs);
	}
 }

void ADialogAICharacter::AnswerToCharacter(FName PlayerLine, TArray<FSubtitle>& SubtitlesToDisplay, float delay)
{
	if (!AILines) return;

	//Retrieve the corresponding line
	FString ContextString;
	FDialog* Dialog = AILines->FindRow<FDialog>(PlayerLine, ContextString);

	ATP_ThirdPersonCharacter* MainChar = Cast<ATP_ThirdPersonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Dialog && MainChar)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;

		TimerDel.BindUFunction(this, FName("Talk"), Dialog->SFX, Dialog->Subtitles);

		//Talk to the player after the delay time has passed
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, delay, false);
	}
}

