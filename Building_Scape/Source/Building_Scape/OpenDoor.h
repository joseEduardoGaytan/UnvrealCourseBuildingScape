// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_SCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Method that abstracts common code for the door closing and opening
	virtual void AnimateDoor(float DeltaTime, float FinalYaw, float AnimationQuickness);
	// Method that opens the door
	virtual void OpenDoor(float DeltaTime);
	// Method that closes the door
	virtual void CloseDoor(float DeltaTime);

private:
	float InitialYaw;
	float CurrentYaw;

	// Place it as a property in Unreal Editor
	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;

	// Door Behaviour
	// Door Close And Opening Velocity
	UPROPERTY(EditAnywhere)
	float OpenDoorTime = 1.5f;
	UPROPERTY(EditAnywhere)
	float CloseDoorTime = 4.f;

	// Door Close Delay
	float DoorLastOpened = 0.f;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5f;
		
};
