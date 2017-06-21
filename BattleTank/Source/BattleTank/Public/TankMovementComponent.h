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
	
	UFUNCTION(BlueprintCallable, Category = "Speed")
		float GetSpeedCurrent();

private:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void BeginPlay() override;

	//Called from the pathfinding logic by the AI controllers
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	void Move(float DeltaTime);

	//UHoverTankTrack *HoverTrack = nullptr;	
	FVector MoveDirection;

	UPROPERTY(EditAnywhere)
	float m_Speed = 10000.0f;

	
	//keep track of move input 
	FVector InputSaved;
	//keep track of boost input
	float BoostInput = 0.0f;

	//accelaration current value
	float AccelerationCurrent = 0.0f;
	//accelaration limit min value
	float AccelerationLimitMin = 0.0f;
	//accelaration limit max value
	float AccelerationLimitMax = 1.0f;
	//accelaration speed transition between min <-> max
	float AccelerationTransitionSpeed = 1.0f;

	//boost current value
	float BoostAccelerationCurrent;
	//boost limit min value
	float BoostAccelerationLimitMin = 1.0f;	
	//boost limit max value
	UPROPERTY(EditAnywhere, Category = "Boost")
	float BoostAccelerationLimitMax = 5.0f;
	//boost speed transition between min <-> max
	UPROPERTY(EditAnywhere, Category = "Boost")
	float BoostTransitionSpeed = 25.0f;		
	//boost duration max
	UPROPERTY(EditAnywhere, Category = "Boost")
	float BoostDurationMax = 1.0f;
	float BoostDurationCurrent;
	//speed used when increase BoostDurationCurrent
	UPROPERTY(EditAnywhere, Category = "Boost")
	float BoostDurationIncreaseSpeed = 1.0f;
	//speed used when decrease BoostDurationCurrent
	UPROPERTY(EditAnywhere, Category = "Boost")
	float BoostDurationDecreaseSpeed = 1.5f;
};
