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

	/*UFUNCTION(BlueprintCallable, Category = "Input")
		void InputBoost(float AxisValue);*/
	UFUNCTION(BlueprintCallable, Category = "Input")
		void StartBoost();
	
	UFUNCTION(BlueprintCallable, Category = "Speed")
		float GetSpeedCurrent();

	UFUNCTION(BlueprintCallable, Category = "Boost")
		bool IsBoosting() const;

	UFUNCTION(BlueprintCallable, Category = "Boost")
		int32 GetBoostGaugeCurrent() const;

	UFUNCTION(BlueprintCallable, Category = "Boost")
		int32 GetBoostGaugeMax() const;

	UFUNCTION(BlueprintCallable, Category = "Boost")
		int32 GetBoostGaugeReloadTimePercentage() const;

private:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void BeginPlay() override;

	//Called from the pathfinding logic by the AI controllers
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	void Move(float DeltaTime);

	void UpdateBoost(float DeltaTime);

	//UHoverTankTrack *HoverTrack = nullptr;	
	FVector MoveDirection;

	UPROPERTY(EditAnywhere)
	float m_Speed = 10000.0f;

	
	//keep track of move input 
	FVector InputSaved;

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
	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	float m_BoostDurationMax = 0.2f;
	float m_BoostDurationCurrent;
	//max amplitude for sin or cos function
	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	float m_BoostValueAmplitudeMax = 100000.0f;
	//clamped value for boost
	//determine the speed transition between 0 & max amplitude for sin & cos function 
	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	float m_BoostValueClampedMax = 25000.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Boost")
		int32 m_BoostGaugeNumberMax = 5;
		int32 m_BoostGaugeNumberCurrent;

		//in seconds
	UPROPERTY(EditDefaultsOnly, Category = "Boost")
		float m_BoostGaugeReloadTimeMax = 1.0f;
	//in seconds
	float m_BoostGaugeReloadTimeCurrent;

};
