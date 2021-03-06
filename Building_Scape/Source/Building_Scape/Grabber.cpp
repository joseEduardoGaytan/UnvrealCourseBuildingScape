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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Checking for Physics handle component
	this->FindPhysicsHandle();

	// Init input component
	this->InitInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Move the object we are holding if any.
	if (this->PhysicsHandle && this->PhysicsHandle->GrabbedComponent)
	{
		FVector LineTraceEnd = this->GetPlayerReach();
		this->PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::FindPhysicsHandle()
{
	// We use get owner as it is the same component and we use the dimond notation to bring the only
	// UPhysicsHandleComponent already added to the component	
	AActor* Owner = GetOwner();
	this->PhysicsHandle = Owner->FindComponentByClass<UPhysicsHandleComponent>();

	if (!this->PhysicsHandle)
	{		
		UE_LOG(LogTemp, Error, TEXT("The Physics handle is not set on %s"), *Owner->GetName());
	}	
}

void UGrabber::InitInputComponent()
{
	AActor* Owner = GetOwner();
	this->InputComponent = Owner->FindComponentByClass<UInputComponent>();

	// Subscribe input component to the callbacks
	if (this->InputComponent)
	{
		this->InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		this->InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input comopnent not sent %s"), *Owner->GetName());
	}
}

void UGrabber::Grab()
{	
	// Try and reach any actors with physics body collision channel already set
	// If we hit something then attach the physics handle
	FHitResult Hit;
	FVector LineTraceEnd = this->GetPlayerReach();
	bool bObjectHits = this->IsAPhysicsBodyToReach(OUT Hit);
	AActor* ActorHit = (bObjectHits) ? Hit.GetActor() : nullptr;
	
	//UE_LOG(LogTemp, Warning, TEXT("Actor hit: %d"), bObjectHits);
	if (this->PhysicsHandle && bObjectHits && ActorHit)
	{
		UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
		//UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *ActorHit->GetName());
		// Attach physics handle
		this->PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

void UGrabber::Release()
{
	// remove/release the physics handle if any object
	if (this->PhysicsHandle)
	{
		UPrimitiveComponent* GrabbedComponent = this->PhysicsHandle->GetGrabbedComponent();
		if (GrabbedComponent)
		{
			this->PhysicsHandle->ReleaseComponent();
		}
	}
}

bool UGrabber::IsAPhysicsBodyToReach(FHitResult& OutHit) const
{
	// Get players viewport
	// Declare out parameters
	UWorld* CurrentWorld = GetWorld();
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	CurrentWorld->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotator);

	// Draw a line from player showing the reach	
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotator.Vector() * this->Reach);

	/*DrawDebugLine(
		CurrentWorld,
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);*/

	// Ray-cast out to a certain distance (Reach)	
	FCollisionObjectQueryParams CollisionQueryParams(ECollisionChannel::ECC_PhysicsBody);
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	bool bObjectHits = CurrentWorld->LineTraceSingleByObjectType(
		OUT OutHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		CollisionQueryParams,
		TraceParams
	);

	return bObjectHits;

	// See what it hits
	/*UE_LOG(LogTemp, Warning, TEXT("Actor hit: %d"), bObjectHits);
	if (bObjectHits && Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *Hit.GetActor()->GetName());
	}*/
}

FVector UGrabber::GetPlayerReach() const
{
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

	return LineTraceEnd;
}
