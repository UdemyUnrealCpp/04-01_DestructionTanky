// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;		
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	m_currentHealth = m_startingHealth;
}

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

	//UE_LOG(LogTemp, Warning, TEXT("%s had %f healt but take %i / %f damage = %i"), *TankName, m_lastHealth, DamageToApply, DamageAmount, this->m_currentHealth);
	return DamageToApply;
}

float ATank::GetHealthPercent() const
{
	return (float)(this->m_currentHealth) / (float)(this->m_startingHealth);
}
