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

	//this->move
	float DirectionX = FMath::Clamp<float>(ForwardAxisValue, -1.0f, 1.0f);
	float DirectionY = FMath::Clamp<float>(RightAxisValue, -1.0f, 1.0f);
	float DirectionZ = 0.0f;

	this->MoveDirection = FVector(DirectionX, DirectionY, DirectionZ);

	this->AddInputVector(this->MoveDirection.GetSafeNormal());
}

void UTankMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move(DeltaTime);
}

void UTankMovementComponent::Move(float DeltaTime)
{
	FVector Input = this->ConsumeInputVector();
	float InputSize = Input.SizeSquared();

	//acceleration
	
	FVector ForceApplied = UpdatedComponent->GetForwardVector() * Input.X;
	ForceApplied += UpdatedComponent->GetRightVector() * Input.Y;

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *ForceApplied.ToString());

	this->MoveUpdatedComponent(ForceApplied * m_Speed * DeltaTime, this->GetOwner()->GetActorRotation(), true);
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



