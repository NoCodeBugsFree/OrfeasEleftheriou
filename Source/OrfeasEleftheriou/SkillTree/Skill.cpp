// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill.h"


// Sets default values
ASkill::ASkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));

	SphereComp->SetCollisionProfileName(FName("BlockAll"));

	SetRootComponent(SphereComp);

	// Initializing the movement component and its properties
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComp"));

	ProjectileMovementComp->ProjectileGravityScale = 0.f;
	ProjectileMovementComp->InitialSpeed = 3000.f;

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(FName("ParticleComp"));
	ParticleComp->SetupAttachment(SphereComp);

	InitialLifeSpan = 10.f;
}

void ASkill::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Used in order to have a visual feedback in the editor when we 
	// assign a new particle
	if (ProjectileFX)
	{
		ParticleComp->SetTemplate(ProjectileFX);
		ParticleComp->Activate();
	}
}

// Called when the game starts or when spawned
void ASkill::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->OnComponentHit.AddDynamic(this, &ASkill::OnHit);

	if (ProjectileFX)
	{
		ParticleComp->SetTemplate(ProjectileFX);
		ParticleComp->Activate();
	}
}

// Called every frame
void ASkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkill::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ProjectileCollisionFX)
	{
		// Activate the collision FX and start the destroy timer
		ParticleComp->SetTemplate(ProjectileCollisionFX);
		ParticleComp->Activate(true);
	}

 	FTimerHandle TimerHandle;
 	FTimerDelegate TimerDel;
 
 	TimerDel.BindLambda([&]()
 	{
 		Destroy();
 	});
 
 	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, DestroyDelay, false);
}

