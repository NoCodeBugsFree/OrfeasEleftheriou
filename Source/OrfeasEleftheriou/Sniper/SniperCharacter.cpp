// Fill out your copyright notice in the Description page of Project Settings.

#include "SniperCharacter.h"
#include "Sniper/SniperProjectile.h"
#include "DrawDebugHelpers.h"
#include "Sniper/SniperEnemyCharacter.h"

ASniperCharacter::ASniperCharacter()
{
	GetMesh1P()->SetOnlyOwnerSee(false);
	GetFP_GunMesh()->SetOnlyOwnerSee(false);
	
	// Create the spring arm comp
	ThirdPersonSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("ThirdPersonSpringArmComp"));
	ThirdPersonSpringArmComp->TargetArmLength = 150.f;
	ThirdPersonSpringArmComp->SocketOffset = FVector(0.f, 0.f, 90.f);
	ThirdPersonSpringArmComp->SetupAttachment(GetRootComponent());

	// Create the third person camera comp
	ThirdPersonCameraComp = CreateDefaultSubobject<UCameraComponent>(FName("ThirdPersonCameraComp"));
	ThirdPersonCameraComp->SetupAttachment(ThirdPersonSpringArmComp);
}

void ASniperCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Tells the controller to search for an owned camera component to view through when used as a view target
	GetController()->bFindCameraComponentWhenViewTarget = true;
}

void ASniperCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Set up gameplay key bindings

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFP_FirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFP_FirstPersonCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	/** SniperFire  */
	InputComponent->BindAction("SniperFire", IE_Pressed, this, &ASniperCharacter::OnSniperFire);
}

void ASniperCharacter::ResetActiveCamera()
{
	// Activate the first person camera
	GetFirstPersonCameraComponent()->Activate();
	GetThirdPersonCameraComponent()->Deactivate();

	// Transition to the first person camera
	Cast<APlayerController>(GetController())->SetViewTarget(this);
}

void ASniperCharacter::OnSniperFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();

		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetFP_GunMesh()->GetSocketLocation("Muzzle");
		
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			ASniperProjectile* SpawnedProjectile = World->SpawnActor<ASniperProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			
			if (SpawnedProjectile)
			{
				ASniperEnemyCharacter* EnemyToBeKilled = nullptr; // ------
															      //      -					
				if (HitsAnEnemy(SpawnedProjectile, EnemyToBeKilled)) //  <- HitsAnEnemy sets the EnemyToBeKilled pointer by reference
				{
					SpawnedProjectile->SetEnemyToKill(EnemyToBeKilled);

					// FVector LineEnd = SpawnLocation + SpawnRotation.Vector() * 10000.f;
					// DrawDebugLine(World, SpawnLocation, LineEnd, FColor::MakeRandomColor(), true, 10.f, 0, 1.f);

					// Dilate the time
					UGameplayStatics::SetGlobalTimeDilation(World, TimeDilationMultiplier);
					
					// Change the activate camera
					ActivateThirdPersonCamera();

					/** delay... and activate projectile camera  */
					ActivateProjectileCameraInDelay(SpawnedProjectile, World);
				}
			}
		}
	}

	// Plays the fire shot sound and animation montage
	PlayFireFX();
}

void ASniperCharacter::ActivateProjectileCameraInDelay(ASniperProjectile* SpawnedProjectile, UWorld* const World)
{
	// Activate Projectile Camera
	// Create a timer handle and a timer delegate
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;

	// Assign the corresponding UFUNCTION to the timer delegate
	TimerDel.BindUFunction(this, FName("ActivateProjectileCamera"), SpawnedProjectile);

	// Fire the delegate after she specified delay
	World->GetTimerManager().SetTimer(TimerHandle, TimerDel, ThirdPersonToProjectileTransitionDelay, false);
}

void ASniperCharacter::PlayFireFX()
{
	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void ASniperCharacter::ActivateThirdPersonCamera()
{
	// Deactivates the first person camera
	GetFirstPersonCameraComponent()->Deactivate();

	// Activates the third person camera
	GetThirdPersonCameraComponent()->Activate();
}

void ASniperCharacter::ActivateProjectileCamera(ASniperProjectile* Projectile)
{
	if (Projectile)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->SetViewTargetWithBlend(Projectile, ThirdPersonCameraToProjectileCameraBlendTime);
		}
	}
}

bool ASniperCharacter::HitsAnEnemy(ASniperProjectile* Projectile, ASniperEnemyCharacter*& HitEnemy)
{
	HitEnemy = Cast<ASniperEnemyCharacter>(GetHitActor(Projectile));

	return (HitEnemy) ? true : false;
}

AActor* ASniperCharacter::GetHitActor(ASniperProjectile* Projectile)
{
	FHitResult HitResult;

	FVector StartLocation = Projectile->GetActorLocation();
	FVector EndLocation = StartLocation + (Projectile->GetActorForwardVector() * RaycastLength);

	FCollisionQueryParams CollisionParams;

	// Ignore the character and the projectile
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(Projectile);

	// Perform a raycast - search for pawns only
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Pawn, CollisionParams);

	return HitResult.GetActor();
}
