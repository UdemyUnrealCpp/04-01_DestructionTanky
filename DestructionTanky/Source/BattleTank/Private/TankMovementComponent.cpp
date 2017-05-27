// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"

#include "TankTrack.h"

#include "TankMovementComponent.h"


void UTankMovementComponent::Initialise(UTankTrack *LeftTrackToSet, UTankTrack *RightTrackToSet)
{
	this->LeftTrack = LeftTrackToSet;
	this->RightTrack = RightTrackToSet;	
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }

	this->LeftTrack->SetThrottle(Throw);
	this->RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }

	this->LeftTrack->SetThrottle(Throw);
	this->RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	FVector TankForward = this->GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector AIFowardIntention = MoveVelocity.GetSafeNormal();

	float ForwardThrow = FVector::DotProduct(TankForward, AIFowardIntention);
	IntendMoveForward(ForwardThrow);

	FVector RightThrow = FVector::CrossProduct(TankForward, AIFowardIntention);
	IntendTurnRight(RightThrow.Z);
}



