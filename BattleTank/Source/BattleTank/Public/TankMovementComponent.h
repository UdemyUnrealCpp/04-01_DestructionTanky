// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

/**
 *  Responsible for driving the tank track
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()	
	
public:
	UTankMovementComponent();

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void Initialise(UHoverTankTrack *HoverTrackToSet);

	UFUNCTION(BlueprintCallable, Category = "Input")
		void InputMoveDirection(float ForwardAxisValue, float RightAxisValue);

private:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	//Called from the pathfinding logic by the AI controllers
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	void Move(float DeltaTime);

	//UHoverTankTrack *HoverTrack = nullptr;	
	FVector MoveDirection;

	UPROPERTY(EditAnywhere)
	float m_Speed = 100.0f;

	//0-1
	float m_AccelerationValue;
	
	UPROPERTY(EditAnywhere)
	float m_TimeToAccelerate = 1.0f;
};
