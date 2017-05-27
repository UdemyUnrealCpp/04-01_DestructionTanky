// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	//-1 is max downward speed, and +1 is max up movement
	void Elevate(float RelativeSpeed);
	void SetRotationPitch(float RotationPitch);
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float m_maxDegreesPerSecond = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float m_elevationInDegreeMin = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float m_elevationInDegreeMax = 40;
};
