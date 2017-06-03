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

	UFUNCTION(BlueprintCallable, Category = "Input")
		void InputBoost(float AxisValue);

private:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	//Called from the pathfinding logic by the AI controllers
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	void Move(float DeltaTime);

	//UHoverTankTrack *HoverTrack = nullptr;	
	FVector MoveDirection;

	UPROPERTY(EditAnywhere)
	float m_Speed = 10000.0f;

	UPROPERTY(EditAnywhere)
	float m_BoostSpeedMultiply = 10.0f;


	float BoostValue = 1.0f;

	FVector InputSaved;
	float Acceleration = 0.0f;
	float AccelerationTransitionSpeed = 1.0f;

	float BoostAccelerationLimit = 10.0f;
	float BoostAcceleration = 1.0f;
	float BoostTransitionSpeed = 10.0f;
	float BoostInput = 0.0f;
	float BoostDurationLimit = 1.0f;
	float BoostDurationCurrent = 1.0f;
	
	UPROPERTY(EditAnywhere)
	float m_TimeToAccelerate = 1.0f;
};
