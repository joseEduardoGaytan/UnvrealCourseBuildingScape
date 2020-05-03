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
			
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// float InPitch => y, InYaw => z, InRoll => x
	// we can set the rotation directly to the CurrentRotation object
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	// Rotate the door with linear interpolation, which we are using exponential interpolation indeed.
	// starting Yaw until target yaw from 0-1*
	float AnimationQuickness = 0.02f;
	float RotateTo = FMath::Lerp(CurrentRotation.Yaw, this->TargetYaw, AnimationQuickness);
	// Rotate the door 90 degrees
	FRotator NewRotation(CurrentRotation);
	NewRotation.Yaw = RotateTo;
	GetOwner()->SetActorRotation(NewRotation);
}

