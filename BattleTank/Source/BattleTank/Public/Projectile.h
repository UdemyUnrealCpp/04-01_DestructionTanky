// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	void LaunchProjectile(float Speed);
	void LaunchProjectile(FVector Direction, float Speed);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Setup")
		UProjectileMovementComponent* m_projectileMovement = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
		USphereComponent* m_collisionComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
		UStaticMeshComponent * m_meshComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
		UParticleSystemComponent* m_launchBlast = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
		UParticleSystemComponent* m_impactBlast = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
		URadialForceComponent* m_explosionForce = nullptr;

private:

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Destroy")
	float m_destroyDelay = 10.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float m_projectileDamage = 10.0f;

	void OnTimerExpire();	
};
