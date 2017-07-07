// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"

#include "BattleTankGameModeBase.h"
#include "Tank.h"

#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	this->m_collisionComp = CreateDefaultSubobject<USphereComponent>(FName("Collision Comp"));
	this->SetRootComponent(this->m_collisionComp);
	this->m_collisionComp->SetNotifyRigidBodyCollision(true);

	this->m_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	this->m_projectileMovement->bAutoActivate = false;	

	this->m_meshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh Comp"));
	this->m_meshComp->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	this->m_launchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	this->m_launchBlast->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	this->m_impactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	this->m_impactBlast->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	this->m_impactBlast->bAutoActivate = false;

	this->m_explosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	this->m_explosionForce->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();	

	this->m_collisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	this->m_collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	//FVector vectir = this->GetActorForwardVector();
	//FVector scale = this->GetActorScale() / 2.0f;
	//this->AddActorLocalOffset(FVector(scale.X, 0.0f, scale.Z));
}


void AProjectile::LaunchProjectile(float Speed)
{
	this->m_projectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	this->m_projectileMovement->Activate();
}

void AProjectile::LaunchProjectile(FVector Direction,float Speed)
{
	UE_LOG(LogTemp, Warning, TEXT("LAUNCH __ %s __ S = %f"), *Direction.ToString(), Speed);
	//this->SetActorRotation(Direction.Rotation());
	//FVector scale = this->GetActorScale() / 2.0f;
	//this->AddActorLocalOffset(FVector(scale.X, 0.0f, scale.Z));
	this->m_projectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	this->m_projectileMovement->Activate();

	//this->m_Owner = Owner;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	/*
	if (OtherComp->GetOwner() != nullptr)
	{
		if (OtherComp->GetOwner() == this->m_Owner)
		{
			UE_LOG(LogTemp, Warning, TEXT("HIT OWNER %s = %s"), *this->m_Owner->GetName(), *OtherComp->GetOwner()->GetName())
			return;
		}
	}*/

	//UE_LOG(LogTemp, Warning, TEXT("HIT %s"), *OtherComp->GetOwner()->GetName())
	UE_LOG(LogTemp, Warning, TEXT("HIT %s __ %s"), *HitComponent->GetOwner()->GetName(), *OtherComp->GetOwner()->GetName())
	
	this->m_launchBlast->Deactivate();
	this->m_impactBlast->Activate();

	this->m_explosionForce->FireImpulse();

	this->SetRootComponent(this->m_impactBlast);
	this->m_collisionComp->DestroyComponent();
	this->m_meshComp->DestroyComponent();

	ApplyExplosionForceOnTanks();
	
	


	/*float fDistance = FVector::Distance(OtherComp->GetOwner()->GetActorLocation(), Hit.ImpactPoint);
	float fRadialDamage = FMath::Lerp<float>(this->m_projectileDamage, 0, fDistance / this->m_explosionForce->Radius);
	UE_LOG(LogTemp, Warning, TEXT("location %s __ %s"), *OtherComp->GetOwner()->GetActorLocation().ToString(), *Hit.ImpactPoint.ToString());
	UE_LOG(LogTemp, Warning, TEXT("lerp %f __ %f __ %f"), 0.0f, this->m_projectileDamage, fDistance / this->m_explosionForce->Radius);
	UE_LOG(LogTemp, Warning, TEXT("distance %f __ %f __ %f"), fDistance, fRadialDamage, this->m_explosionForce->Radius);

	UGameplayStatics::ApplyRadialDamage(
		this,
		fRadialDamage,
		this->GetActorLocation(),
		this->m_explosionForce->Radius, //for consistancy
		UDamageType::StaticClass(),
		TArray<AActor*>()//damage all actors
	);*/


	FTimerHandle TimerHandle;
	this->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AProjectile::OnTimerExpire, this->m_destroyDelay, false);
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("OVERLAPP %s __ %s"), *OverlappedComp->GetOwner()->GetName(), *OtherComp->GetOwner()->GetName())
}

void AProjectile::OnTimerExpire()
{
	this->Destroy();
}

void AProjectile::ApplyExplosionForceOnTanks()
{
	ABattleTankGameModeBase* BattleTankGameMode = Cast<ABattleTankGameModeBase>(this->GetWorld()->GetAuthGameMode());
	TArray<APawn*> PawnsControlledByPlayers = BattleTankGameMode->GetPawnsControlledByPlayers();

	UE_LOG(LogTemp, Warning, TEXT("NB Tank %d"), PawnsControlledByPlayers.Num());

	float ExplosionRadius = this->m_explosionForce->Radius;

	for (int i = 0; i < PawnsControlledByPlayers.Num(); ++i)
	{
		FVector TankLocation = PawnsControlledByPlayers[i]->GetActorLocation();
		FVector ProjectileLocation = this->GetActorLocation();
		float DistanceProjectileToTank = FVector::Distance(TankLocation, ProjectileLocation);

		if (DistanceProjectileToTank <= ExplosionRadius)
		{
			float ForceExplosionByDistance = FMath::Lerp<float>(1.0f, 0.0f, DistanceProjectileToTank / ExplosionRadius);

			FVector ForceDirection = (TankLocation - ProjectileLocation).GetSafeNormal();
			ForceDirection.Z = 0.0f;
			UE_LOG(LogTemp, Warning, TEXT("ForceExplosion %f __ ForceDir %s"), ForceExplosionByDistance, *ForceDirection.ToString());

			ATank* Tank = Cast<ATank>(PawnsControlledByPlayers[i]);
			Tank->AddEnvironmentalForce(ForceDirection * ForceExplosionByDistance * this->m_explosionForce->ImpulseStrength);
			Tank->AddDamage(this->m_projectileDamage * ForceExplosionByDistance);

		}
	}
}

