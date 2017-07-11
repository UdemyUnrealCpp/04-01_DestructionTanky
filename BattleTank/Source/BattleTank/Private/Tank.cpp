// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"

#include "TankMovementComponent.h"
#include "TankAimingComponent.h"

#include "Tank.h"




// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;		

	this->m_tankMovement = this->CreateDefaultSubobject<UTankMovementComponent>(FName("TankMovement"));
	this->m_tankAiming = this->CreateDefaultSubobject<UTankAimingComponent>(FName("TankAiming"));
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	TArray<UTankMovementComponent*> MoveComps;
	TArray<UTankAimingComponent*> AimingComps;

	this->GetComponents(MoveComps);
	this->GetComponents(AimingComps);

	if(this->m_tankMovement == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("ERROR"));

	this->m_tankMovement = MoveComps[0];
	this->m_tankAiming = AimingComps[0];

	m_currentHealth = m_startingHealth;

	//this->NotifyHit.AddDynamic(this, &ATank::OnHit);
}

void ATank::Tick(float DeltaSeconds)
{
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);


	//const FVector LocalMove = FVector(1000 * DeltaSeconds, 0.f, 0.f);

	// Move plan forwards (with sweep so we stop when we collide with things)
	//AddActorLocalOffset(LocalMove, true);
	//AddMovementInput(LocalMove);
	//AddActorLocalOffset(this->ConsumeMovementInputVector(), true);
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *LocalMove.ToString());
	FVector Location = this->GetActorLocation();
	Location.Z = this->HeightElevation;
	this->SetActorLocation(Location);
}
/*
float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	FString TankName = this->GetName();

	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);

	int32 DamageToApply = FMath::Clamp<int32>(DamageAmount, 0, this->m_currentHealth);
	float m_lastHealth = this->m_currentHealth;

	this->m_currentHealth -= DamageToApply;
	if (this->m_currentHealth <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s tank died"), *TankName);
		OnDeath.Broadcast();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s had %f healt but take %i / %f damage = %i"), *TankName, m_lastHealth, DamageToApply, DamageAmount, this->m_currentHealth);
	return DamageToApply;
}*/

void ATank::NotifyHit(class UPrimitiveComponent* SendHitComp, AActor* Other, class UPrimitiveComponent* GetHitComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bSelfMoved)
	{
		FVector HitLocationn = HitLocation;

		DrawDebugLine(
			GetWorld(),
			HitLocationn,
			HitLocationn + HitNormal * 10000000.0f,
			FColor(0, 0, 255),
			false, -1, 0,
			12.333
		);

		DrawDebugSphere(
			GetWorld(),
			HitLocationn,
			48,
			8,
			FColor(0, 255, 0)
		);

		DrawDebugSphere(
			GetWorld(),
			Hit.ImpactPoint,
			48,
			8,
			FColor(0, 255, 255)
		);

		float ForceHit = this->m_tankMovement->GetMovementVectorSpeed().Size();
		ForceHit = FMath::Clamp<float>(ForceHit, 0, m_HitSendForceMaxValue);

		ATank *TankHitten = Cast<ATank>(GetHitComp->GetOwner());
		if (TankHitten != nullptr)
		{
			TankHitten->AddEnvironmentalForce(HitNormal * -ForceHit);
			TankHitten->AddDamage((ForceHit / m_HitSendForceMaxValue) * CollisionDamageMax);
		}

		ATank *TankThis = Cast<ATank>(SendHitComp->GetOwner());
		if (TankHitten != nullptr)
		{
			TankThis->AddEnvironmentalForce(HitNormal * ForceHit * m_HitSendForcePercentageReceivedValue);
			TankThis->AddDamage((ForceHit / m_HitSendForceMaxValue) * CollisionDamageMax);		

			UE_LOG(LogTemp, Warning, TEXT("HIT %s __ %s__Normal %s __ FORCE %s %f"), *SendHitComp->GetOwner()->GetName(), *GetHitComp->GetOwner()->GetName(), *HitNormal.ToString(), *this->m_tankMovement->GetMovementVectorSpeed().ToString(), ForceHit);

			TankThis->ResetMovement();
		}
	}
}

void ATank::InitialiseMoveComp()
{
	if (!ensure(this->m_tankMovement))
		return;

	this->m_tankMovement->Initialise();
}

void ATank::InitialiseAimComp(UTankBarrel* barrelToSet)
{
	if (!ensure(this->m_tankAiming))
		return;

	this->m_tankAiming->Initialise(barrelToSet);
}

void ATank::LaunchBoost()
{
	if (!ensure(this->m_tankMovement))
		return;

	this->m_tankMovement->LaunchBoost();
}

void ATank::Fire()
{
	if (!ensure(this->m_tankAiming))
		return;

	if (!ensure(this->m_tankMovement))
		return;

	this->m_tankAiming->Fire();

	//this->m_tankMovement->AddEnvironmentalForce(this->GetActorForwardVector() * 50000.0f);
}

void ATank::Move(float ForwardAxisValue, float RightAxisValue)
{
	if (!ensure(this->m_tankMovement))
		return;

	this->m_tankMovement->Move(ForwardAxisValue, RightAxisValue);
}

float ATank::GetHealthPercent() const
{
	return (float)(this->m_currentHealth) / (float)(this->m_startingHealth);
}

float ATank::GetAmmoPercent() const
{
	return this->m_tankAiming->GetAmmoPercent();
}

float ATank::GetBoostPercent() const
{
	return this->m_tankMovement->GetBoostPercent();
}

void ATank::AddEnvironmentalForce(FVector EnvironmentalForce)
{
	this->m_tankMovement->AddEnvironmentalForce(EnvironmentalForce);
}

void ATank::AddDamage(float Damage)
{
	FString TankName = this->GetName();

	int32 DamagePoints = FPlatformMath::RoundToInt(Damage);

	int32 DamageToApply = FMath::Clamp<int32>(Damage, 0, this->m_currentHealth);
	float m_lastHealth = this->m_currentHealth;

	this->m_currentHealth -= DamageToApply;
	if (this->m_currentHealth <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s tank died"), *TankName);
		OnDeath.Broadcast();
	}

	UE_LOG(LogTemp, Warning, TEXT("%s had %f healt but take %i / %f damage = %i"), *TankName, m_lastHealth, DamageToApply, Damage, this->m_currentHealth);
}

void ATank::ResetMovement()
{
	this->m_tankMovement->ResetMovement();
}


UTankMovementComponent* ATank::GetTankMovementComponent() const
{
	if (!ensure(m_tankMovement)) { return nullptr; }

	return this->m_tankMovement;
}

UTankAimingComponent* ATank::GetTankAimingComponent() const
{
	if (!ensure(m_tankAiming)) { return nullptr; }

	return this->m_tankAiming;
}
