// Fill out your copyright notice in the Description page of Project Settings.
#include "BattleTank.h"

#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

#include "TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::Initialise(UTankBarrel* barrelToSet)
{
	this->m_barrel = barrelToSet;

	m_iNumberAmmoLeft = m_iNumberAmmoMax;
}

void UTankAimingComponent::BeginPlay()
{
	this->m_lastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateAmmo(DeltaTime);

	UpdateFiringState();
}

void UTankAimingComponent::AimAtDirection(FVector Direction)
{
	if (!ensure(m_barrel))
		return;

	m_aimDirection = Direction;
	MoveBarrelTowards(m_aimDirection);
	//MoveTurretTowards(m_aimDirection);
}

void UTankAimingComponent::Fire()
{
	if (this->m_eFiringState == EFiringState::EFiringStatus_LOCKED || this->m_eFiringState == EFiringState::EFiringStatus_AIMING)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%f TANK Fire"), this->GetWorld()->GetTimeSeconds());

		if(!ensure(this->m_barrel)) { return; }

		if(!ensure(this->m_projectileBlueprint)) { return; }

		FVector StartLocation = this->m_barrel->GetSocketLocation(FName("Projectile"));
		//FRotator StartRotation = this->m_barrel->GetSocketRotation(FName("Projectile"));		
		FRotator StartRotation = m_aimDirection.Rotation();
		AProjectile* NewProjectile = this->GetWorld()->SpawnActor<AProjectile>(this->m_projectileBlueprint, StartLocation, StartRotation);
		//NewProjectile->LaunchProjectile(this->m_launchSpeed);
		NewProjectile->LaunchProjectile(m_aimDirection, this->m_launchSpeed, this->GetOwner());
		//NewProjectile = this->GetWorld()->SpawnActor<AProjectile>(this->m_projectileBlueprint, StartLocation, StartRotation);

		//m_lastFireTime = FPlatformTime::Seconds();
		m_iNumberAmmoLeft = m_iNumberAmmoLeft -1;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return this->m_eFiringState;
}

int32 UTankAimingComponent::GetAmmoLeft() const
{
	return this->m_iNumberAmmoLeft;
}

int32 UTankAimingComponent::GetAmmoMax() const
{
	return this->m_iNumberAmmoMax;
}

int32 UTankAimingComponent::GetAmmoReloadTimePercentage() const
{
	float allAmmoReloadTotalInSeconds = this->m_iNumberAmmoMax * this->m_ammoReloadTimeMaxInSeconds;
	float allAmmoReloadCurrentInSeconds = this->m_iNumberAmmoLeft * this->m_ammoReloadTimeMaxInSeconds;
	float Percentage = (allAmmoReloadCurrentInSeconds + this->m_ammoReloadTimeCurrentInSeconds) / allAmmoReloadTotalInSeconds;
	return Percentage * 100.0f;
}

FVector UTankAimingComponent::GetBarrelLocation() const
{
	if (!this->m_barrel) { return FVector::ZeroVector; }

	return this->m_barrel->GetComponentLocation();
}


void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(m_barrel))
		return;

	FRotator AimAsRotator = AimDirection.Rotation();
	this->m_barrel->SetRotationPitch(AimAsRotator.Pitch);

	DrawDebugLine(
		GetWorld(),
		this->m_barrel->GetComponentLocation(),
		this->m_barrel->GetComponentLocation() + AimDirection * 10000000.0f,
		FColor(255, 0, 0),
		false, -1, 0,
		12.333
	);

	/*
	//work out difference between current barrel rotation and aimdirection
	FRotator BarrelRotator = this->m_barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	//UE_LOG(LogTemp, Warning, TEXT("AimAsRotator : %s"), *AimAsRotator.ToString());

	FRotator DeltaRotator = AimAsRotator - BarrelRotator;	

	this->m_barrel->Elevate(DeltaRotator);*/
}


//void UTankAimingComponent::MoveTurretTowards(FVector AimDirection)
//{
//	if (!ensure(m_turret))
//		return;
//	
//	//UE_LOG(LogTemp, Warning, TEXT("AimDirection : %s"), *AimDirection.ToString());
//
//	FRotator AimAsRotator = AimDirection.Rotation();
//	float RotationYaw = AimAsRotator.Yaw;
//
//	FVector Tankroot = this->GetOwner()->GetActorForwardVector();
//	FRotator Tankrootrotation = Tankroot.Rotation();
//	//UE_LOG(LogTemp, Warning, TEXT("Yaw : %f ___ %f"), RotationYaw, Tankrootrotation.Yaw);
//
//
//	RotationYaw = RotationYaw;
//    this->m_turret->SetRotationYaw(RotationYaw - Tankrootrotation.Yaw);
//	//this->GetOwner()->SetActorRelativeRotation(FRotator(0, RotationYaw, 0));
//	
//	
//	
//	//USE RELATIVE SPEED
//
//	//work out difference between current turret rotation and aimdirection
//	/*
//	FRotator TurretRotator = this->m_turret->GetForwardVector().Rotation();
//	FRotator AimAsRotator = AimDirection.Rotation();
//	FRotator DeltaRotator = AimAsRotator - TurretRotator;
//	*/
//	
//	//SOLUTION 1
//	/*
//	//in positive if the delta yaw is more than 180, we need to go to the opposite
//	//exemple go from 0 to 200 -> long range but the shortest is 0 to -140
//	//use -360 to go to the opposite
//	if (DeltaRotator.Yaw > 180.0f)
//	{
//		DeltaRotator.Yaw = DeltaRotator.Yaw - 360.0f;
//	}
//	//exemple go from 0 to -200 -> long range but the shortest is 0 to 140
//	//use +360 to go to the opposite
//	else if (DeltaRotator.Yaw < -180.0f)
//	{
//		DeltaRotator.Yaw = DeltaRotator.Yaw + 360.0f;
//	}
//	this->m_turret->Rotate(DeltaRotator.Yaw);
//	*/
//
//	/*
//	//SOLUTION 2
//	//180.0f is the middle if the delta is less than the middle
//	//Keep the direction
//	if (FMath::Abs(DeltaRotator.Yaw) < 180.0f)
//	{
//		this->m_turret->Rotate(DeltaRotator.Yaw);
//	}
//	else
//	//180.0f is the middle if the delta is more than the middle
//	//the short angle is the opposite (use -1)
//	{
//		this->m_turret->Rotate(-DeltaRotator.Yaw);
//	}*/	
//}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(m_barrel)) { return false; }

	FVector BarrelForward = this->m_barrel->GetForwardVector();
	
	return !BarrelForward.Equals(m_aimDirection, 0.01f);
}

void UTankAimingComponent::UpdateFiringState()
{
	if (m_iNumberAmmoLeft <= 0)
	{
		this->m_eFiringState = EFiringState::EFiringStatus_OUT_OF_AMMO;
	}
	/*else if ((FPlatformTime::Seconds() - m_lastFireTime) < m_fullAmmoReloadTimeMaxInSeconds)
	{
		this->m_eFiringState = EFiringState::EFiringStatus_RELOADING;
	}*/
	else if (IsBarrelMoving())
	{
		this->m_eFiringState = EFiringState::EFiringStatus_AIMING;
	}
	else
	{
		this->m_eFiringState = EFiringState::EFiringStatus_LOCKED;
	}
}

void UTankAimingComponent::UpdateAmmo(float DeltaTime)
{
	if (this->m_iNumberAmmoLeft < m_iNumberAmmoMax)
	{
		this->m_ammoReloadTimeCurrentInSeconds += DeltaTime;

		if (this->m_ammoReloadTimeCurrentInSeconds >= this->m_ammoReloadTimeMaxInSeconds)
		{
			++this->m_iNumberAmmoLeft;
			this->m_ammoReloadTimeCurrentInSeconds = 0.0f;
		}
	}
}

void UTankAimingComponent::Test()
{
	
}
