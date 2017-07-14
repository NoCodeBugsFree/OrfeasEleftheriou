// Fill out your copyright notice in the Description page of Project Settings.

#include "FallingActor.h"


// Sets default values
AFallingActor::AFallingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Initialize a StaticMeshComponent
	SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeSM"));
	SetRootComponent(SM);

	//Enable physics so the cube will fall
	SM->SetSimulatePhysics(true);
	SM->SetNotifyRigidBodyCollision(true);

	//You only need to register the OnHit function
	SM->OnComponentHit.AddDynamic(this, &AFallingActor::OnHit);
}

// Called when the game starts or when spawned
void AFallingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFallingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFallingActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && !IsPendingKill())
	{
		UE_LOG(LogTemp, Error, TEXT("%s reports: OnHit : %s"), *GetName(), *OtherActor->GetName());
	}
}

void AFallingActor::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other && Other != this && !IsPendingKill())
	{
		UE_LOG(LogTemp, Error, TEXT("%s reports: NotifyHit : %s"), *GetName(), *Other->GetName());
	}
}
