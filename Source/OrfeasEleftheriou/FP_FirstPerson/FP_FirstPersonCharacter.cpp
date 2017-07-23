// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FP_FirstPersonCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Inventory/Pickup.h"
#include "Inventory/InventoryPlayerController.h"

static FName WeaponFireTraceIdent = FName(TEXT("WeaponTrace"));
#define COLLISION_WEAPON ECC_GameTraceChannel1

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFP_FirstPersonCharacter

AFP_FirstPersonCharacter::AFP_FirstPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);				// Set so only owner can see mesh
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);	// Attach mesh to FirstPersonCameraComponent
	Mesh1P->bCastDynamicShadow = false;			// Disallow mesh to cast dynamic shadows
	Mesh1P->CastShadow = false;				// Disallow mesh to cast other shadows

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// Only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;		// Disallow mesh to cast dynamic shadows
	FP_Gun->CastShadow = false;			// Disallow mesh to cast other shadows
	FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));

	// Set weapon damage and range
	WeaponRange = 5000.0f;
	WeaponDamage = 500000.0f;

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


void AFP_FirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	/** Initializing our Inventory  */
	Inventory.SetNum(MAX_INVENTORY_ITEMS);
}

void AFP_FirstPersonCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	Raycast();
}

void AFP_FirstPersonCharacter::SetEquippedItem(UTexture2D* Texture)
{
	if (Texture)
	{
		// For this scenario we make the assumption that
		// every pickup has a unique texture.
		// So, in order to set the equipped item we just check every item
		// inside our Inventory. Once we find an item that has the same image as the
		// Texture image we're passing as a parameter we mark that item as CurrentlyEquipped.
		for (auto It = Inventory.CreateIterator(); It; It++)
		{
			if ((*It) && (*It)->GetPickupTexture() && (*It)->GetPickupTexture()->HasSameSourceArt(Texture))
			{
				CurrentlyEquippedItem = *It;
				UE_LOG(LogTemp, Error, TEXT("I've set a new equipped item: %s"), *CurrentlyEquippedItem->GetName());
				break;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Player has clicked an empty inventory slot"));
	}
}

void AFP_FirstPersonCharacter::PickupItem()
{
	if (LastItemSeen)
	{
		// Find the first available slot
		int32 AvailableSlot = Inventory.Find(nullptr);

		if (AvailableSlot != INDEX_NONE)
		{
			// Add the item to the first valid slot we found
			Inventory[AvailableSlot] = LastItemSeen;
			// Destroy the item from the game
			LastItemSeen->Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("You can't carry any more items!"));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFP_FirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);
	
	// Set up gameplay key bindings

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFP_FirstPersonCharacter::OnFire);
	
	// Attempt to enable touch screen movement
	TryEnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFP_FirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFP_FirstPersonCharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFP_FirstPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFP_FirstPersonCharacter::LookUpAtRate);

	/* Templated version of EquipWeapon binding */
	//We tell the compiler that we pick the EWeaponType::Knife explicitly
	PlayerInputComponent->BindAction("EquipKnife", IE_Pressed, this, &AFP_FirstPersonCharacter::EquipWeapon<EWeaponType::WT_Knife>);
	//We tell the compiler that we pick the EWeaponType::Rifle explicitly
	PlayerInputComponent->BindAction("EquipRifle", IE_Pressed, this, &AFP_FirstPersonCharacter::EquipWeapon<EWeaponType::WT_Rifle>);
	//We tell the compiler that we pick the EWeaponType::Handgun explicitly
	PlayerInputComponent->BindAction("EquipHandgun", IE_Pressed, this, &AFP_FirstPersonCharacter::EquipWeapon<EWeaponType::WT_HandGun>);

	/** Pickup  */
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AFP_FirstPersonCharacter::PickupItem);
	
	// DropItem
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &AFP_FirstPersonCharacter::DropEquippedItem);

	/** Inventory  */
	// old inventory binding
	// PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AFP_FirstPersonCharacter::HandleInventoryInput);
	
	// Pause inventory binding 
	FInputActionBinding InventoryBinding;
	//We need this bind to execute on pause state
	InventoryBinding.bExecuteWhenPaused = true;
	InventoryBinding.ActionDelegate.BindDelegate(this, FName("HandleInventoryInput"));
	InventoryBinding.ActionName = FName("Inventory");
	InventoryBinding.KeyEvent = IE_Pressed;

	//Binding the Inventory action
	InputComponent->AddActionBinding(InventoryBinding);


	// -----------------------------------------------------------------------------------
	// ------- Implementing Action Bindings with parameters ------------------------------
	// -----------------------------------------------------------------------------------

	// First param action binding...

	// Declaring an action binding
	FInputActionBinding ActionBindingOneParam;
	// Specifying the associated action name and the key event
	ActionBindingOneParam.ActionName = FName("OneParamInput");
	ActionBindingOneParam.KeyEvent = IE_Pressed;

	// Creating a handler which binds to the given function with a fixed int parameter
	FInputActionHandlerSignature OneParamActionHandler;

	// Binding the function named OneParamFunction to our handler
	// The first parameter (this) means that the handler will search the given function inside the current class
	OneParamActionHandler.BindUFunction(this, FName("OneParamFunction"), FirstIntParam);

	// Associating our action binding with our new delegate
	ActionBindingOneParam.ActionDelegate = OneParamActionHandler;

	// Performing the actual binding...
	InputComponent->AddActionBinding(ActionBindingOneParam);

	// Second Param - identical code to the first param action bind but with a different function and parameters!

	FInputActionBinding ActionBindingTwoParams;

	ActionBindingTwoParams.ActionName = FName("TwoParamsInput");
	ActionBindingTwoParams.KeyEvent = IE_Pressed;

	FInputActionHandlerSignature TwoParamsActionHandler;
	TwoParamsActionHandler.BindUFunction(this, FName("TwoParamsFunction"), SecondIntParam, FloatParam);

	ActionBindingTwoParams.ActionDelegate = TwoParamsActionHandler;

	InputComponent->AddActionBinding(ActionBindingTwoParams);

	// Binding the change parameters function
	InputComponent->BindAction("ChangeParameters", IE_Pressed, this, &AFP_FirstPersonCharacter::ChangeParameters);
}

void AFP_FirstPersonCharacter::OnFire()
{
	// Play a sound if there is one
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// Try and play a firing animation if specified
	if(FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if(AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

	// Now send a trace from the end of our gun to see if we should hit anything
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	// Calculate the direction of fire and the start location for trace
	FVector CamLoc;
	FRotator CamRot;
	PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector ShootDir = CamRot.Vector();

	FVector StartTrace = FVector::ZeroVector;
	if (PlayerController)
	{
		FRotator UnusedRot;
		PlayerController->GetPlayerViewPoint(StartTrace, UnusedRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		StartTrace = StartTrace + ShootDir * ((GetActorLocation() - StartTrace) | ShootDir);
	}

	// Calculate endpoint of trace
	const FVector EndTrace = StartTrace + ShootDir * WeaponRange;

	// Check for impact
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	// Deal with impact
	AActor* DamagedActor = Impact.GetActor();
	UPrimitiveComponent* DamagedComponent = Impact.GetComponent();

	// If we hit an actor, with a component that is simulating physics, apply an impulse
	if ((DamagedActor != NULL) && (DamagedActor != this) && (DamagedComponent != NULL) && DamagedComponent->IsSimulatingPhysics())
	{
		DamagedComponent->AddImpulseAtLocation(ShootDir*WeaponDamage, Impact.Location);
	}
}

void AFP_FirstPersonCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// If touch is already pressed check the index. If it is not the same as the current touch assume a second touch and thus we want to fire
	if (TouchItem.bIsPressed == true)
	{
		if( TouchItem.FingerIndex != FingerIndex)
		{
			OnFire();			
		}
	}
	else 
	{
		// Cache the finger index and touch location and flag we are processing a touch
		TouchItem.bIsPressed = true;
		TouchItem.FingerIndex = FingerIndex;
		TouchItem.Location = Location;
		TouchItem.bMoved = false;
	}
}

void AFP_FirstPersonCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// If we didn't record the start event do nothing, or this is a different index
	if((TouchItem.bIsPressed == false) || ( TouchItem.FingerIndex != FingerIndex) )
	{
		return;
	}

	// If the index matches the start index and we didn't process any movement we assume we want to fire
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}

	// Flag we are no longer processing the touch event
	TouchItem.bIsPressed = false;
}

void AFP_FirstPersonCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// If we are processing a touch event and this index matches the initial touch event process movement
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (GetWorld() != nullptr)
		{
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
			{
				FVector MoveDelta = Location - TouchItem.Location;
				FVector2D ScreenSize;
				ViewportClient->GetViewportSize(ScreenSize);
				FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
				if (FMath::Abs(ScaledDelta.X) >= (4.0f / ScreenSize.X))
				{
					TouchItem.bMoved = true;
					float Value = ScaledDelta.X * BaseTurnRate;
					AddControllerYawInput(Value);
				}
				if (FMath::Abs(ScaledDelta.Y) >= (4.0f / ScreenSize.Y))
				{
					TouchItem.bMoved = true;
					float Value = ScaledDelta.Y* BaseTurnRate;
					AddControllerPitchInput(Value);
				}
				TouchItem.Location = Location;
			}
			TouchItem.Location = Location;
		}
	}
}

void AFP_FirstPersonCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFP_FirstPersonCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFP_FirstPersonCharacter::TurnAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFP_FirstPersonCharacter::LookUpAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FHitResult AFP_FirstPersonCharacter::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(WeaponFireTraceIdent, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams);
	
	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, true, 10.f, (uint8)0, 1.f);

	return Hit;
}

void AFP_FirstPersonCharacter::TryEnableTouchscreenMovement(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFP_FirstPersonCharacter::BeginTouch);
	PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFP_FirstPersonCharacter::EndTouch);
	PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFP_FirstPersonCharacter::TouchUpdate);	
}

void AFP_FirstPersonCharacter::EquipWeapon(EWeaponType EWeaponToEquip)
{
	switch (EWeaponToEquip)
	{
		case EWeaponType::WT_Knife:
		{
			UE_LOG(LogTemp, Error, TEXT("Equipped knife"));
			// more logic here
			break;
		}
		case EWeaponType::WT_Rifle:
		{
			UE_LOG(LogTemp, Error, TEXT("Equipped rifle"));
			// more logic here
			break;
		}
		case EWeaponType::WT_HandGun:
		{
			UE_LOG(LogTemp, Error, TEXT("Equipped handgun"));
			// more logic here
			break;
		}
	}
}

void AFP_FirstPersonCharacter::CalculatePrimeNumbers()
{
	// Performing the prime numbers calculations in the game thread...

	ThreadingTest::CalculatePrimeNumbers(MaxPrime);

	GLog->Log("--------------------------------------------------------------------");
	GLog->Log("End of prime numbers calculation on game thread");
	GLog->Log("--------------------------------------------------------------------");

}

void AFP_FirstPersonCharacter::CalculatePrimeNumbersAsync()
{
	/*Create a new Task and pass as a parameter our MaxPrime
	Then, tell that Task to execute in the background.

	The FAutoDeleteAsyncTask will make sure to delete the task when it's finished.

	Multithreading requires cautious handle of the available threads, in order to avoid
	race conditions and strange bugs that are not easy to solve

	Fortunately, UE4 contains a class (FAutoDeleteAsyncTask) which handles everything by itself
	and the programmer is able to perform async operations without any real effort.*/

	(new FAutoDeleteAsyncTask<PrimeCalculationAsyncTask>(MaxPrime))->StartBackgroundTask();
}

void AFP_FirstPersonCharacter::DropEquippedItem()
{
	if (CurrentlyEquippedItem)
	{
		int32 IndexOfItem;
		if (Inventory.Find(CurrentlyEquippedItem, IndexOfItem))
		{
			// The location of the drop
			FVector DropLocation = GetActorLocation() + (GetActorForwardVector() * 200);

			// Making a transform with default rotation and scale. Just setting up the location
			// that was calculated above
			FTransform Transform; Transform.SetLocation(DropLocation);

			//Default actor spawn parameters
			FActorSpawnParameters SpawnParams;

			//Spawning our pickup
			APickup* PickupToSpawn = GetWorld()->SpawnActor<APickup>(CurrentlyEquippedItem->GetClass(), Transform, SpawnParams);
			
			if (PickupToSpawn)
			{
				// Unreference the item we've just placed
				Inventory[IndexOfItem] = nullptr;
			}
		}
	}
}

void AFP_FirstPersonCharacter::Raycast()
{
	// Calculating start and end location
	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndLocation = StartLocation + (FirstPersonCameraComponent->GetForwardVector() * RaycastRange);

	FHitResult RaycastHit;

	// Raycast should ignore the character
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	// Raycast
	GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, CQP);
	
	APickup* Pickup = Cast<APickup>(RaycastHit.GetActor());

	if (LastItemSeen && LastItemSeen != Pickup)
	{
		// If our character sees a different pickup then disable the glowing effect on the previous seen item
		LastItemSeen->SetGlowEffect(false);
	}

	if (Pickup)
	{
		// Enable the glow effect on the current item
		LastItemSeen = Pickup;
		Pickup->SetGlowEffect(true);
	} // Re-Initialize 
	else LastItemSeen = nullptr;
}

void AFP_FirstPersonCharacter::HandleInventoryInput()
{
	AInventoryPlayerController* InventoryPlayerController = Cast<AInventoryPlayerController>(GetController());
	if (InventoryPlayerController)
	{
		InventoryPlayerController->HandleInventoryInput();
	}
}

void AFP_FirstPersonCharacter::OneParamFunction(int32 Param)
{
	//printing the given parameter
	GLog->Log("The parameter you've entered:" + FString::FromInt(Param));
}

void AFP_FirstPersonCharacter::TwoParamsFunction(int32 IntParam, float FloatParameter)
{
	//printing the given parameters
	GLog->Log("Input with two parameters. First param: " + FString::FromInt(IntParam) + " Second param: " + FString::SanitizeFloat(FloatParam));
}

void AFP_FirstPersonCharacter::ChangeParameters()
{
	//Choosing a different random param!
	SecondIntParam = FMath::Rand();

	FloatParam = FMath::RandRange(-1000.f, 1000.f);

	GLog->Log("Changed params: " + FString::FromInt(SecondIntParam) + " " + FString::SanitizeFloat(FloatParam));


	//Search all the action bindings until we find the binding with the two parameters
	for (int32 i = 0; i < InputComponent->GetNumActionBindings() - 1; i++)
	{
		if (InputComponent->GetActionBinding(i).ActionName.IsEqual("TwoParamsInput"))
		{
			//Declaring a new binding with the same action name and key event as the TwoParamsInput initial action binding
			FInputActionBinding NewActionBinding;
			NewActionBinding.ActionName = FName("TwoParamsInput");
			NewActionBinding.KeyEvent = IE_Pressed;

			//Creating an identical handler with the same bind as before
			FInputActionHandlerSignature NewDelegate;
			//However, this bind contains our new values!
			NewDelegate.BindUFunction(this, FName("TwoParamsFunction"), SecondIntParam, FloatParam);

			//Associating our handler with our action binding
			NewActionBinding.ActionDelegate = NewDelegate;

			//The GetActionBinding function returns the action binding by reference
			//This means that in the following line we replace the old binding (which contains old values)
			//with a new binding (which contains the updated values)
			InputComponent->GetActionBinding(i) = NewActionBinding;

			GLog->Log("changed the action binding");
		}
	}
}