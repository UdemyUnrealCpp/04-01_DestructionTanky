// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

//enum for aiming state
UENUM()
enum class EFiringState : uint8
{
	EFiringStatus_LOCKED,
	EFiringStatus_AIMING,
	EFiringStatus_RELOADING,
	EFiringStatus_OUT_OF_AMMO,
};

//forward declaration
class UTankBarrel; 
class UTankTurret;
class AProjectile;

//Hold barrel's properties and Elevate Method
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrel* barrelToSet);

	void AimAtDirection(FVector Direction);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();
	
	EFiringState GetFiringState() const;	

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetAmmoLeft() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetAmmoMax() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetAmmoReloadTimePercentage() const;

	FVector GetBarrelLocation() const;

	float GetAmmoPercent() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
		EFiringState m_eFiringState = EFiringState::EFiringStatus_RELOADING;

	UPROPERTY(EditAnywhere, Category = "Setup")
		TSubclassOf<AProjectile> m_projectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float m_launchSpeed = 100000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float m_ammoReloadTimeMaxInSeconds = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		int32 m_iNumberAmmoMax = 5;

private:
	// Sets default values for this component's properties
	UTankAimingComponent();

	UTankBarrel* m_barrel = nullptr;

	double m_lastFireTime = 0;	
	FVector m_aimDirection;

	int32 m_iNumberAmmoLeft;
	float m_ammoReloadTimeCurrentInSeconds;

	void MoveBarrelTowards(FVector AimDirection);
	//void MoveTurretTowards(FVector AimDirection);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void BeginPlay() override;

	void UpdateFiringState();

	void UpdateAmmo(float DeltaTime);

	bool IsBarrelMoving();

	float Delay;
};
