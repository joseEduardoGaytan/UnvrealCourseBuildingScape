// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_SCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Grabber callback functions
	void Grab();
	void Release();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// The "arms" distance
	float Reach = 100.f;
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr; // Initialize with a null pointer in order to not brake unreal
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	// User scope reach
	FVector GetPlayerReach() const;

	void FindPhysicsHandle();
	void InitInputComponent();

	// Return the first Actor within reach with physics body
	bool IsAPhysicsBodyToReach(FHitResult &OutHit) const;	
			
};
