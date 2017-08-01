// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

ABomb::ABomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(GetStaticMeshComponent());

	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComp"));
	ProjectileMovementComp->bShouldBounce = true;

	// Since we need to replicate some functionality
	// for this actor, we need to mark it as true
	SetReplicates(true);
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();

	// Register that function that will be called in any bounce event
	ProjectileMovementComp->OnProjectileBounce.AddDynamic(this, &ABomb::OnProjectileBounce);
}

void ABomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Tell the engine that we wish to replicate the bIsArmed variable
	DOREPLIFETIME(ABomb, bIsArmed);
}

void ABomb::OnRep_IsArmed()
{
	// Will get called when the bomb is armed 
	// from the authority client
	if (bIsArmed)
	{
		ArmBomb();
	}
}

void ABomb::ArmBomb()
{
	if (bIsArmed && GetStaticMeshComponent())
	{
		// Chance the base color of the static mesh to red
		UMaterialInstanceDynamic* DynamicMAT = GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0);

		DynamicMAT->SetVectorParameterValue(FName("Color"), FLinearColor::Red);
	}
}

void ABomb::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	// If the bomb is not armed and we have authority,
	// arm it and perform a delayed explosion
	if (!bIsArmed && Role == ROLE_Authority)
	{
		bIsArmed = true;
		ArmBomb();

		PerformDelayedExplosion(FuseTime);
	}
}

void ABomb::PerformDelayedExplosion(float ExplosionDelay)
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("Explode"));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, ExplosionDelay, false);
}

void ABomb::Explode()
{
	SimulateExplosionFX();

	// We won't use any specific damage types in our case
	TSubclassOf<UDamageType> DmgType;
	// Do not ignore any actors
	TArray<AActor*> IgnoreActors;

	// This will eventually call the TakeDamage function that we have overridden in the Character class
	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius, DmgType, IgnoreActors, this, GetInstigatorController());

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;

	TimerDel.BindLambda([&]()
	{
		Destroy();
	});

	// Destroy the actor after 0.3 seconds. 
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.3f, false);
}

void ABomb::SimulateExplosionFX_Implementation()
{
	if (ExplosionFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetTransform(), true);
	}
}

