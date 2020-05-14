// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Just remark our keyword
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Checking for Physics handle component
	// We use get owner as it is the same component and we use the dimond notation to bring the only
	// UPhysicsHandleComponent already added to the component
	this->PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (this->PhysicsHandle)
	{		
		UE_LOG(LogTemp, Warning, TEXT("Actual Physics Handler: %s"), *this->PhysicsHandle->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Physics handle is not set on %s"), *GetOwner()->GetName());
	}
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get players viewport
	// Declare out parameters
	UWorld* CurrentWorld = GetWorld();
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;
	
	CurrentWorld->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotator);

	// Logging out to test
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s; And Direction: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotator.ToString());

	// Draw a line from player showing the reach
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotator.Vector() * this->Reach);

	DrawDebugLine(
		CurrentWorld,
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);

	// Ray-cast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionObjectQueryParams CollisionQueryParams(ECollisionChannel::ECC_PhysicsBody);
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	bool bObjectHits = CurrentWorld->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		CollisionQueryParams,
		TraceParams
	);

	// See what it hits
	/*UE_LOG(LogTemp, Warning, TEXT("Actor hit: %d"), bObjectHits);
	if (bObjectHits && Hit.GetActor())
	{		
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *Hit.GetActor()->GetName());
	}*/
}

