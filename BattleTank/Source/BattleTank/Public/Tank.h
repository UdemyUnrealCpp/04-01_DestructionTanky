// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

class UTankMovementComponent;
class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()	
	
public:

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void InitialiseMoveComp();

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void InitialiseAimComp(UTankBarrel* barrelToSet);

		void LaunchBoost();
		void Fire();
		void Move(float ForwardAxisValue, float RightAxisValue);

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealthPercent() const;



	FTankDelegate OnDeath;
	
	UFUNCTION(BlueprintCallable, Category = "CompRef")
	UTankMovementComponent* GetTankMovementComponent() const;
	UFUNCTION(BlueprintCallable, Category = "CompRef")
	UTankAimingComponent* GetTankAimingComponent() const;

private:
	// Sets default values for this pawn's properties
	ATank();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	
	virtual void BeginPlay() override;	

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		int32 m_startingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category="Health")
		int32 m_currentHealth;

	UPROPERTY(EditDefaultsOnly)
		float HeightElevation = 250.0f;	

	UPROPERTY(VisibleAnywhere, Category = "Component")
		UTankMovementComponent* m_tankMovement;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		UTankAimingComponent* m_tankAiming;
};

