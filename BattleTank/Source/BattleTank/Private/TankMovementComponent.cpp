// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"

#include "HoverTankTrack.h"

#include "TankMovementComponent.h"


UTankMovementComponent::UTankMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankMovementComponent::Initialise(UHoverTankTrack *HoverTrackToSet)
{
	//this->HoverTrack = HoverTrackToSet;
}

void UTankMovementComponent::BeginPlay()
{
	this->BoostAccelerationCurrent = this->BoostAccelerationLimitMin;
	this->BoostDurationCurrent = this->BoostDurationMax;
	this->AccelerationCurrent = this->AccelerationLimitMin;
}

void UTankMovementComponent::InputMoveDirection(float ForwardAxisValue, float RightAxisValue)
{
	//if (!ensure(HoverTrack)) { return; }

	//this->HoverTrack->SetThrottleVector(FVector(ForwardAxisValue, RightAxisValue, 0.0f));

	/*if (ForwardAxisValue >= -0.1f && ForwardAxisValue <= 0.1f
		&& RightAxisValue >= -0.1f && RightAxisValue <= 0.1f)
		return;*/

	//this->move
	float DirectionX = FMath::Clamp<float>(ForwardAxisValue, -1.0f, 1.0f);
	float DirectionY = FMath::Clamp<float>(RightAxisValue, -1.0f, 1.0f);
	float DirectionZ = 0.0f;

	this->MoveDirection = FVector(DirectionX, DirectionY, DirectionZ);

	this->AddInputVector(this->MoveDirection.GetSafeNormal());
	//this->InputSaved = this->MoveDirection.GetSafeNormal();

	
}

void UTankMovementComponent::InputBoost(float AxisValue)
{
	BoostInput = AxisValue;
}

float UTankMovementComponent::GetSpeedCurrent()
{
	return m_Speed * this->AccelerationCurrent * this->BoostAccelerationCurrent;
}

void UTankMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move(DeltaTime);
}

void UTankMovementComponent::Move(float DeltaTime)
{
	FVector Input = this->ConsumeInputVector();
	//FVector Input = this->InputSaved;
	float InputSize = Input.SizeSquared();

	//acceleration + input

	if (InputSize > 0.25f)
	{
		this->InputSaved = Input;
		this->AccelerationCurrent += DeltaTime * this->AccelerationTransitionSpeed;
	}
	else
	{
		this->AccelerationCurrent -= DeltaTime * this->AccelerationTransitionSpeed;
	}

	this->AccelerationCurrent = FMath::Clamp<float>(this->AccelerationCurrent, this->AccelerationLimitMin, this->AccelerationLimitMax);

	if (this->AccelerationCurrent <= 0.0f)
	{
		this->InputSaved = FVector::ZeroVector;
	}
	
	FVector InputForceApplied = UpdatedComponent->GetForwardVector() * this->InputSaved.X;
	InputForceApplied += UpdatedComponent->GetRightVector() * this->InputSaved.Y;
	InputForceApplied = InputForceApplied.GetSafeNormal();

	//boost
	if (BoostInput > 0.0f)
	{
		this->BoostAccelerationCurrent += DeltaTime * this->BoostTransitionSpeed;
		BoostDurationCurrent -= DeltaTime * this->BoostDurationDecreaseSpeed;
	}
	else
	{
		//this->BoostAccelerationCurrent -= DeltaTime * this->BoostTransitionSpeed;
		this->BoostAccelerationCurrent = 1.0f;
		BoostDurationCurrent += DeltaTime * this->BoostDurationIncreaseSpeed;
	}

	this->BoostAccelerationCurrent = FMath::Clamp<float>(this->BoostAccelerationCurrent, this->BoostAccelerationLimitMin, this->BoostAccelerationLimitMax);
	BoostDurationCurrent = FMath::Clamp<float>(BoostDurationCurrent, 0.0f, BoostDurationMax);	
	
	if (BoostDurationCurrent <= 0.0f)
	{
		this->BoostAccelerationCurrent = 1.0f;
	}

	//UE_LOG(LogTemp, Warning, TEXT("A %f __ BA %f __ D %f"), this->AccelerationCurrent, this->BoostAccelerationCurrent, BoostDurationCurrent);
	
	FVector NewSpeedVector = InputForceApplied * this->GetSpeedCurrent();
	//UE_LOG(LogTemp, Warning, TEXT("speed = %s "), *NewSpeedVector.ToString());

	this->MoveUpdatedComponent(NewSpeedVector * DeltaTime, this->GetOwner()->GetActorRotation(), true);
	this->UpdateComponentVelocity();
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	FVector TankForward = this->GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector AIFowardIntention = MoveVelocity.GetSafeNormal();

	/*AI disable
	float ForwardThrow = FVector::DotProduct(TankForward, AIFowardIntention);
	IntendMoveForward(ForwardThrow);

	FVector RightThrow = FVector::CrossProduct(TankForward, AIFowardIntention);
	IntendTurnRight(RightThrow.Z);
	*/
}



