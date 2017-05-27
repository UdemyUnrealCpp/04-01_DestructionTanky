// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"


void UTankBarrel::Elevate(float RelativeSpeed)
{
	//move the barrel the right amount this frame
	//given a max elevation speed, and the frame time	
	//UE_LOG(LogTemp, Warning, TEXT("Barrel Elevate Called at speed : %f"), RelativeSpeed);

	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0f, 1.0f);

	float ElevationChange = RelativeSpeed * this->m_maxDegreesPerSecond * this->GetWorld()->DeltaTimeSeconds;
	float RawNewElevation = this->RelativeRotation.Pitch + ElevationChange;
	float Elevation = FMath::Clamp<float>(RawNewElevation, this->m_elevationInDegreeMin, this->m_elevationInDegreeMax);
	this->SetRelativeRotation(FRotator(Elevation, 0.0f, 0.0f));
}



void UTankBarrel::SetRotationPitch(float RotationPitch)
{
	float RawNewElevation = RotationPitch;
	float PitchClamped = FMath::Clamp<float>(RawNewElevation, this->m_elevationInDegreeMin, this->m_elevationInDegreeMax);
	this->SetRelativeRotation(FRotator(PitchClamped, 0, 0));
}

