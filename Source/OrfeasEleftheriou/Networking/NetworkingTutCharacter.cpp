// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkingTutCharacter.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "Networking/Bomb.h"

ANetworkingTutCharacter::ANetworkingTutCharacter()
{
	CharText = CreateDefaultSubobject<UTextRenderComponent>(FName("CharText"));
	CharText->SetRelativeLocation(FVector(0, 0, 100));
	CharText->SetupAttachment(GetRootComponent());
}

void ANetworkingTutCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ThrowBomb", IE_Pressed, this, &ANetworkingTutCharacter::AttempToSpawnBomb);
}

void ANetworkingTutCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitHealth();
	InitBombCount();
}

void ANetworkingTutCharacter::OnRep_Health()
{
	UpdateCharText();
}

void ANetworkingTutCharacter::OnRep_BombCount()
{
	UpdateCharText();
}

void ANetworkingTutCharacter::InitHealth()
{
	Health = MaxHealth;
	UpdateCharText();
}

void ANetworkingTutCharacter::InitBombCount()
{
	BombCount = MaxBombCount;
	UpdateCharText();
}

void ANetworkingTutCharacter::UpdateCharText()
{
	// Create a string that will display the health and bomb count values
	FString NewText = FString("Health: ") + FString::SanitizeFloat(Health) + FString(" Bomb Count: ") + FString::FromInt(BombCount);

	// Set the created string to the text render comp
	CharText->SetText(FText::FromString(NewText));
}

void ANetworkingTutCharacter::ServerTakeDamage_Implementation(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

bool ANetworkingTutCharacter::ServerTakeDamage_Validate(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return true;
}

void ANetworkingTutCharacter::AttempToSpawnBomb()
{
	if (HasBombs())
	{
		if (Role < ROLE_Authority)
		{
			ServerSpawnBomb();
		}
		else SpawnBomb();
	}
}

void ANetworkingTutCharacter::SpawnBomb()
{
	// Decrease the bomb count and update the text in the local client
	// OnRep_BombCount will be called in every other client
	BombCount--;
	UpdateCharText();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = this;
	SpawnParameters.Owner = GetController();
	
	// Spawn the bomb
	GetWorld()->SpawnActor<ABomb>(BombActorBP, GetActorLocation() + GetActorForwardVector() * 200, GetActorRotation(), SpawnParameters);
}

void ANetworkingTutCharacter::ServerSpawnBomb_Implementation()
{
	SpawnBomb();
}

bool ANetworkingTutCharacter::ServerSpawnBomb_Validate()
{
	return true;
}

float ANetworkingTutCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// Decrease the character's hp 

	Health -= Damage;
	if (Health <= 0)
	{
		InitHealth();
	}

	// Call the update text on the local client
	// OnRep_Health will be called in every other client so the character's text
	// will contain a text with the right values
	UpdateCharText();

	return Health;
}

void ANetworkingTutCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Tell the engine to call the OnRep_Health and OnRep_BombCount each time
	// a variable changes
	DOREPLIFETIME(ANetworkingTutCharacter, Health);
	DOREPLIFETIME(ANetworkingTutCharacter, BombCount);
}

