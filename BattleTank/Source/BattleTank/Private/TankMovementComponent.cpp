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
		this->Acceleration += DeltaTime * this->AccelerationTransitionSpeed;
	}
	else
	{
		this->Acceleration -= DeltaTime * this->AccelerationTransitionSpeed;
	}

	this->Acceleration = FMath::Clamp<float>(this->Acceleration, 0.0f, 1.0f);

	if (this->Acceleration <= 0.0f)
	{
		this->InputSaved = FVector::ZeroVector;
	}
	
	FVector ForceApplied = UpdatedComponent->GetForwardVector() * this->InputSaved.X;
	ForceApplied += UpdatedComponent->GetRightVector() * this->InputSaved.Y;

	//boost
	if (BoostInput > 0.0f)
	{
		this->BoostAcceleration += DeltaTime * this->BoostTransitionSpeed;
		BoostDurationCurrent -= DeltaTime;
	}
	else
	{
		this->BoostAcceleration -= DeltaTime * this->BoostTransitionSpeed * 2.0f;
		BoostDurationCurrent += (DeltaTime * 0.5f);
	}
	this->BoostAcceleration = FMath::Clamp<float>(this->BoostAcceleration, 1.0f, BoostAccelerationLimit);
	BoostDurationCurrent = FMath::Clamp<float>(BoostDurationCurrent, 0.0f, BoostDurationLimit);

	if (BoostDurationCurrent <= 0.0f)
	{
		this->BoostAcceleration = 1.0f;
	}


	UE_LOG(LogTemp, Warning, TEXT("BA %f __ D %f"), this->BoostAcceleration, BoostDurationCurrent);

	this->MoveUpdatedComponent(ForceApplied * m_Speed * this->Acceleration * this->BoostAcceleration * DeltaTime, this->GetOwner()->GetActorRotation(), true);
	this->UpdateComponentVelocity();

	BoostValue = 1.0f;
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



