// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	this->InitialYaw = GetOwner()->GetActorRotation().Yaw;
	this->CurrentYaw = this->InitialYaw;
	// The target yaw already set by the user in unreal editor plus the initial yaw
	this->TargetYaw += this->InitialYaw;

	// report when a pressure plate is not set
	if (!this->PressurePlate)
	{
		FString ActorName = this->GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("The Object: %s has the open Door component on it, but no Pressure Plate set yet"), *ActorName);
	}

	// Set the pawn that open doors, in the past we have to set it at run time by ejecting the components
	// and set it as a property while we are playing
	this->ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
			
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check for nulls
	if (this->PressurePlate && this->ActorThatOpens && this->PressurePlate->IsOverlappingActor(this->ActorThatOpens))
	{
		//Open the door
		this->OpenDoor(DeltaTime);
	}
	else if(this->PressurePlate && this->ActorThatOpens)
	{
		this->CloseDoor(DeltaTime);
	}

}

void UOpenDoor::OpenDoor(float DeltaTime) 
{
	this->AnimateDoor(DeltaTime, this->TargetYaw, 2.f);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{		
	this->AnimateDoor(DeltaTime, this->InitialYaw, 2.f);	
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

