// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"

#include "HoverTankTrack.h"

#include "TankMovementComponent.h"


UTankMovementComponent::UTankMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankMovementComponent::Initialise()
{
	//nothing for the moment
	this->BoostAccelerationCurrent = 0.0f;
	this->AccelerationCurrent = this->AccelerationLimitMin;
	this->m_BoostDurationCurrent = 0.0f;
	this->m_BoostGaugeNumberCurrent = m_BoostGaugeNumberMax;
}

void UTankMovementComponent::BeginPlay()
{
	this->BoostAccelerationCurrent = 0.0f;
	this->AccelerationCurrent = this->AccelerationLimitMin;
	this->m_BoostDurationCurrent = 0.0f;
	this->m_BoostGaugeNumberCurrent = m_BoostGaugeNumberMax;
	this->m_EnvironmentalForceBoostCounterAttackCurrent = this->m_EnvironmentalForceBoostCounterAttackMin;
}

void UTankMovementComponent::Move(float ForwardAxisValue, float RightAxisValue)
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

/*
void UTankMovementComponent::InputBoost(float AxisValue)
{
	BoostInput = AxisValue;
}*/

void UTankMovementComponent::LaunchBoost()
{
	float MovementVectorSpeedSizeSquared = this->m_EnvironmentalForce.SizeSquared();

	if (this->m_BoostGaugeNumberCurrent <= 0)
		return;	

	if (this->InputSaved.SizeSquared() <= 0.0f && MovementVectorSpeedSizeSquared <= 0.0f)
		return;

	/*
	if (MovementVectorSpeedSizeSquared > 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("1 speed = %s "), *this->m_EnvironmentalForce.ToString());


		FVector Dir = this->m_EnvironmentalForce.GetSafeNormal();
		FVector NewForce;
		NewForce = this->m_EnvironmentalForce + (Dir * -1.0f * m_BoostValueClampedMax);

		if (Dir.X > 0.0f)
		{
			if (NewForce.X <= 0.0f)
				NewForce.X = 0.0f;
		}
		else if (Dir.X < 0.0f)
		{
			if (NewForce.X >= 0.0f)
				NewForce.X = 0.0f;
		}

		if (Dir.Y > 0.0f)
		{
			if (NewForce.Y <= 0.0f)
				NewForce.Y = 0.0f;
		}
		else if (Dir.Y < 0.0f)
		{
			if (NewForce.Y >= 0.0f)
				NewForce.Y = 0.0f;
		}

		if (Dir.Z > 0.0f)
		{
			if (NewForce.Z <= 0.0f)
				NewForce.Z = 0.0f;
		}
		else if (Dir.Z < 0.0f)
		{
			if (NewForce.Z >= 0.0f)
				NewForce.Z = 0.0f;
		}

		this->m_EnvironmentalForce = NewForce;


		UE_LOG(LogTemp, Warning, TEXT("2 speed = %s "), *this->m_EnvironmentalForce.ToString());
	}
	*/

	m_BoostDurationCurrent = m_BoostDurationMax;
	--this->m_BoostGaugeNumberCurrent;
	UE_LOG(LogTemp, Warning, TEXT("BOOST START"));
}

float UTankMovementComponent::GetSpeedCurrent()
{
	//return m_Speed * this->AccelerationCurrent * this->BoostAccelerationCurrent;
	return (m_Speed * this->AccelerationCurrent) + this->BoostAccelerationCurrent;
}

bool UTankMovementComponent::IsBoosting() const
{
	return m_BoostDurationCurrent > 0.0f ? true : false;
}

int32 UTankMovementComponent::GetBoostGaugeCurrent() const
{
	return this->m_BoostGaugeNumberCurrent;
}

int32 UTankMovementComponent::GetBoostGaugeMax() const
{
	return this->m_BoostGaugeNumberMax;
}

int32 UTankMovementComponent::GetBoostGaugeReloadTimePercentage() const
{
	float allGaugeReloadTotalInSeconds = this->m_BoostGaugeNumberMax * this->m_BoostGaugeReloadTimeMax;
	float allGaugeReloadCurrentInSeconds = this->m_BoostGaugeNumberCurrent * this->m_BoostGaugeReloadTimeMax;
	float Percentage = (allGaugeReloadCurrentInSeconds + this->m_BoostGaugeReloadTimeCurrent) / allGaugeReloadTotalInSeconds;
	return Percentage * 100.0f;
}

void UTankMovementComponent::AddEnvironmentalForce(FVector EnvironmentalForce)
{
	this->m_EnvironmentalForce += EnvironmentalForce;

	this->m_EnvironmentalForce.X = FMath::Clamp(this->m_EnvironmentalForce.X, -this->m_EnvironmentalForceMaxValue, this->m_EnvironmentalForceMaxValue);
	this->m_EnvironmentalForce.Y = FMath::Clamp(this->m_EnvironmentalForce.Y, -this->m_EnvironmentalForceMaxValue, this->m_EnvironmentalForceMaxValue);
	this->m_EnvironmentalForce.Z = FMath::Clamp(this->m_EnvironmentalForce.Z, -this->m_EnvironmentalForceMaxValue, this->m_EnvironmentalForceMaxValue);

	UE_LOG(LogTemp, Warning, TEXT("ENV FORCE %s"), *this->m_EnvironmentalForce.ToString());

}

void UTankMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMovement(DeltaTime);

	UpdateBoost(DeltaTime);

	UpdateEnvironmentalForce(DeltaTime);
}

void UTankMovementComponent::UpdateMovement(float DeltaTime)
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
	/*
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
	}*/

	/*
	if (BoostInput > 0.0f && m_bIsBoost == false)
	{
		m_bIsBoost = true;
		m_BoostDurationCurrent = m_BoostDurationMax;
		UE_LOG(LogTemp, Warning, TEXT("BOOST START"));
	}*/


	if (m_BoostDurationCurrent > 0.0f)
	{
		m_BoostDurationCurrent -= DeltaTime;

		float SinValue = 0.0f;
		float Percentage = m_BoostDurationCurrent / m_BoostDurationMax;

		if (m_BoostDurationCurrent <= 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("BOOST END"));
			m_EnvironmentalForceBoostCounterAttackCurrent = m_EnvironmentalForceBoostCounterAttackMin;
		}
		else
		{
			SinValue = FMath::Sin(Percentage * PI) * m_BoostValueAmplitudeMax;
			m_EnvironmentalForceBoostCounterAttackCurrent = m_EnvironmentalForceBoostCounterAttackMax;
		}		

		SinValue = FMath::Clamp(SinValue, 0.0f, m_BoostValueClampedMax);
		BoostAccelerationCurrent = SinValue;

		//UE_LOG(LogTemp, Warning, TEXT("BOOST Dur %f __ SIN %f __ SPEED %f"), m_BoostDurationCurrent, SinValue, BoostAccelerationCurrent);
	}

	//UE_LOG(LogTemp, Warning, TEXT("A %f __ BA %f __ D %f"), this->AccelerationCurrent, this->BoostAccelerationCurrent, BoostDurationCurrent);
	
	this->m_MovementVectorSpeed = InputForceApplied * this->GetSpeedCurrent() + m_EnvironmentalForce;
	//UE_LOG(LogTemp, Warning, TEXT("speed = %s "), *this->m_MovementVectorSpeed.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("speed = %s "), *this->m_EnvironmentalForce.ToString());


	this->MoveUpdatedComponent(m_MovementVectorSpeed * DeltaTime, this->GetOwner()->GetActorRotation(), true);
	this->UpdateComponentVelocity();
}

void UTankMovementComponent::UpdateBoost(float DeltaTime)
{
	if (this->m_BoostGaugeNumberCurrent < this->m_BoostGaugeNumberMax)
	{
		this->m_BoostGaugeReloadTimeCurrent += DeltaTime;

		if (this->m_BoostGaugeReloadTimeCurrent >= this->m_BoostGaugeReloadTimeMax)
		{
			++this->m_BoostGaugeNumberCurrent;
			this->m_BoostGaugeReloadTimeCurrent = 0.0f;
		}
	}
}

void UTankMovementComponent::UpdateEnvironmentalForce(float DeltaTime)
{
	if (this->m_EnvironmentalForce.SizeSquared() > 0.0f)
	{
		FVector NewEnvironmentalForce = this->m_EnvironmentalForce;

		if (NewEnvironmentalForce.X > 0.0f)
		{
			NewEnvironmentalForce.X -= m_EnvironmentalForceDecreaseValue * DeltaTime * m_EnvironmentalForceBoostCounterAttackCurrent;

			if (NewEnvironmentalForce.X <= 0.0f)
			{
				NewEnvironmentalForce.X = 0.0f;
			}
		}
		else if (NewEnvironmentalForce.X < 0.0f)
		{
			NewEnvironmentalForce.X += m_EnvironmentalForceDecreaseValue * DeltaTime * m_EnvironmentalForceBoostCounterAttackCurrent;

			if (NewEnvironmentalForce.X >= 0.0f)
			{
				NewEnvironmentalForce.X = 0.0f;
			}
		}

		if (NewEnvironmentalForce.Y > 0.0f)
		{
			NewEnvironmentalForce.Y -= m_EnvironmentalForceDecreaseValue * DeltaTime * m_EnvironmentalForceBoostCounterAttackCurrent;

			if (NewEnvironmentalForce.Y <= 0.0f)
			{
				NewEnvironmentalForce.Y = 0.0f;
			}
		}
		else if (NewEnvironmentalForce.Y < 0.0f)
		{
			NewEnvironmentalForce.Y += m_EnvironmentalForceDecreaseValue * DeltaTime * m_EnvironmentalForceBoostCounterAttackCurrent;

			if (NewEnvironmentalForce.Y >= 0.0f)
			{
				NewEnvironmentalForce.Y = 0.0f;
			}
		}

		NewEnvironmentalForce.Z = 0.0f;

		this->m_EnvironmentalForce = NewEnvironmentalForce;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Env FORCE __ %s"), *this->m_EnvironmentalForce.ToString());
}

void UTankMovementComponent::ResetMovement()
{
	this->m_BoostDurationCurrent = 0.0f;
	this->BoostAccelerationCurrent = 0.0f;
	this->AccelerationCurrent = this->AccelerationLimitMin;
}

FVector UTankMovementComponent::GetMovementVectorSpeed() const
{
	return this->m_MovementVectorSpeed;
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



