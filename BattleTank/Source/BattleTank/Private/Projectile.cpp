// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//this->m_projectileComponent = CreateDefaultSubobject<USceneComponent>(FName("Projectile"));
	//this->SetRootComponent(this->m_projectileComponent);

	this->m_collisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	this->SetRootComponent(this->m_collisionMesh);
	//this->m_collisionMesh->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	this->m_collisionMesh->SetNotifyRigidBodyCollision(true);

	this->m_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	this->m_projectileMovement->bAutoActivate = false;	

	this->m_launchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	this->m_launchBlast->AttachToComponent(this->m_collisionMesh, FAttachmentTransformRules::KeepRelativeTransform);

	this->m_impactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	this->m_impactBlast->AttachToComponent(this->m_collisionMesh, FAttachmentTransformRules::KeepRelativeTransform);
	this->m_impactBlast->bAutoActivate = false;

	this->m_explosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	this->m_explosionForce->AttachToComponent(this->m_collisionMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();	

	this->m_collisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	this->m_collisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	//FVector vectir = this->GetActorForwardVector();
	//FVector scale = this->GetActorScale() / 2.0f;
	//this->AddActorLocalOffset(FVector(scale.X, 0.0f, scale.Z));
}


void AProjectile::LaunchProjectile(float Speed)
{
	this->m_projectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	this->m_projectileMovement->Activate();
}

void AProjectile::LaunchProjectile(FVector Direction,float Speed, AActor* Owner)
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
	this->m_collisionMesh->DestroyComponent();

	UGameplayStatics::ApplyRadialDamage(
		this,
		this->m_projectileDamage,
		this->GetActorLocation(),
		this->m_explosionForce->Radius, //for consistancy
		UDamageType::StaticClass(),
		TArray<AActor*>() //damage all actors
	);


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


