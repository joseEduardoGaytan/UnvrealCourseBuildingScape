// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	this->FindAudioComponent();

	this->InitialYaw = GetOwner()->GetActorRotation().Yaw;
	this->CurrentYaw = this->InitialYaw;
	// The target yaw already set by the user in unreal editor plus the initial yaw
	this->OpenAngle += this->InitialYaw;

	// report when a pressure plate is not set
	if (!this->PressurePlate)
	{
		FString ActorName = this->GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("The Object: %s has the open Door component on it, but no Pressure Plate set yet"), *ActorName);
	}
					
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check for nulls	
	if (this->TotalMassOfActors() > this->MassToOpenTheDoor)
	{
		//Open the door
		this->OpenDoor(DeltaTime);
		// Get the time when the door opened last time
		this->DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if(this->PressurePlate)
	{
		// add a delay in function of the last time the door was opened
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float DiffTime = CurrentTime - this->DoorLastOpened;
		if (DiffTime >= this->DoorCloseDelay)
		{
			this->CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) 
{
	this->AnimateDoor(DeltaTime, this->OpenAngle, this->DoorOpenSpeed);

	// Play sound and toggle the sound flag when door opens
	if (this->AudioComponent && !this->OpenDoorSoundPlayed)
	{		
		this->OpenDoorSoundPlayed = true;
		this->AudioComponent->Play();
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{		
	this->AnimateDoor(DeltaTime, this->InitialYaw, this->DoorCloseSpeed);

	// Play sound and toggle the sound flag when door closes
	if (this->AudioComponent && this->OpenDoorSoundPlayed)
	{
		this->OpenDoorSoundPlayed = false;
		this->AudioComponent->Play();
	}

}

void UOpenDoor::AnimateDoor(float DeltaTime, float FinalYaw, float AnimationQuickness)
{
	// float InPitch => y, InYaw => z, InRoll => x
	// we can set the rotation directly to the CurrentRotation object
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	// Rotate the door with linear interpolation, which we are using exponential interpolation indeed.
	// starting Yaw until target yaw from 0-1*
	//float AnimationQuickness = 2.f;
	// Exponential interpolation
	//float RotateTo = FMath::Lerp(CurrentRotation.Yaw, this->TargetYaw, AnimationQuickness);

	// Lineal Interpolation
	//float RotateTo = FMath::FInterpConstantTo(CurrentRotation.Yaw, this->TargetYaw, DeltaTime, 45);
	// It doesn't matter the rate, it always open at the same time
	this->CurrentYaw = FMath::FInterpTo(this->CurrentYaw, FinalYaw, DeltaTime, AnimationQuickness);

	// Rotate the door 90 degrees
	/*FRotator NewRotation(CurrentRotation);
	NewRotation.Yaw = this->CurrentYaw;*/
	CurrentRotation.Yaw = this->CurrentYaw;
	GetOwner()->SetActorRotation(CurrentRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find all Overlapping actors
	TArray<AActor*> OverlappingActors;
	if (this->PressurePlate)
	{
		this->PressurePlate->GetOverlappingActors(OUT OverlappingActors);
		
		for(AActor* OverlappingActor: OverlappingActors)
		{
			float ActorsMass = OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			TotalMass += ActorsMass;
		}
	}

	// Add up their masses
	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	AActor* CurrentComponent = GetOwner();
	this->AudioComponent = CurrentComponent->FindComponentByClass<UAudioComponent>();

	// Prevent the sounds to play at start
	if (this->AudioComponent)
	{
		this->AudioComponent->Stop();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing Audio component!"), *CurrentComponent->GetName());
	}
}
