// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	//// Pointer is the memory address of something
	//FString Log = TEXT("Hello");
	//FString* PtrLog = &Log;

	//(*PtrLog).Len(); // To access the properties and member of the pointed object
	//PtrLog->Len(); // Another way to access to the reference members
	//
	//// UE_LOG(LogTemp, Warning, TEXT("%s"), *Log);	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), **PtrLog);	
	
	FString ActorName = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *ActorName);

}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

