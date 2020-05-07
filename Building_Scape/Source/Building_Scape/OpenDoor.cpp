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
			
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->PressurePlate && this->ActorThatOpens && this->PressurePlate->IsOverlappingActor(this->ActorThatOpens))
	{
		//Open the door
		this->OpenDoor(DeltaTime);
	}

}

void UOpenDoor::OpenDoor(float DeltaTime) 
{
	// float InPitch => y, InYaw => z, InRoll => x
	// we can set the rotation directly to the CurrentRotation object
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	// Rotate the door with linear interpolation, which we are using exponential interpolation indeed.
	// starting Yaw until target yaw from 0-1*
	float AnimationQuickness = 2.f;
	// Exponential interpolation
	//float RotateTo = FMath::Lerp(CurrentRotation.Yaw, this->TargetYaw, AnimationQuickness);

	// Lineal Interpolation
	//float RotateTo = FMath::FInterpConstantTo(CurrentRotation.Yaw, this->TargetYaw, DeltaTime, 45);
	// It doesn't matter the rate, it always open at the same time
	this->CurrentYaw = FMath::FInterpTo(this->CurrentYaw, this->TargetYaw, DeltaTime, AnimationQuickness);

	// Rotate the door 90 degrees
	/*FRotator NewRotation(CurrentRotation);
	NewRotation.Yaw = this->CurrentYaw;*/
	CurrentRotation.Yaw = this->CurrentYaw;
	GetOwner()->SetActorRotation(CurrentRotation);
}

