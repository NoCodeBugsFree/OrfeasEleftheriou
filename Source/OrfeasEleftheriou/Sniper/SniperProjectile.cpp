// Fill out your copyright notice in the Description page of Project Settings.

#include "SniperProjectile.h"
#include "Components/SphereComponent.h"
#include "Sniper/SniperEnemyCharacter.h"

// Sets default values
ASniperProjectile::ASniperProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it																						
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASniperProjectile::OnOverlapBegin);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 5000.f;

	// Don't clamp the max speed of the projectile
	ProjectileMovement->MaxSpeed = 0.f;

	ProjectileMovement->bRotationFollowsVelocity = true;

	// Disable the bounce effect on projectiles
	ProjectileMovement->bShouldBounce = false;

	// We want this projectile to exist until it hits something so don't set an initial life span
	InitialLifeSpan = 2.f;

	// Create the spring arm component
	ProjectileSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("ProjectileSpringArmComp"));

	// Attach it to our root component
	ProjectileSpringArmComp->SetupAttachment(GetRootComponent());

	// Create the camera component
	ProjectileCameraComp = CreateDefaultSubobject<UCameraComponent>(FName("ProjectileCameraComp"));

	// Attach it to our spring arm component
	ProjectileCameraComp->SetupAttachment(ProjectileSpringArmComp);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ProjectileMesh->SetupAttachment(RootComponent);
}

void ASniperProjectile::ApplyVelocityMultiplier()
{
	ProjectileMovement->Velocity *= VelocityMultiplier;

	// We need to explicitly tell the projectile movement to update it's velocity after we adjust it's value
	ProjectileMovement->UpdateComponentVelocity();
}

// Called when the game starts or when spawned
void ASniperProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASniperProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bActivatedTransition && EnemyToKill && (EnemyToKill->GetActorLocation() - GetActorLocation()).Size() <= DeathCameraTransitionDistance)
	{
		// We're near the enemy, enable the camera transition
		EnemyToKill->EnableCameraTransition();
		bActivatedTransition = true;
	}
}

void ASniperProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this))
	{	
		if (OtherActor->IsA<ASniperEnemyCharacter>())
		{
			Cast<ASniperEnemyCharacter>(OtherActor)->Die();
		}
		Destroy();
	}
}