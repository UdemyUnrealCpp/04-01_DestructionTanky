// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();

	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::SetThrottle(float fThrottle)
{
	this->m_throttleCurrent = FMath::Clamp<float>(this->m_throttleCurrent + fThrottle, -1.0f, 1.0f);
}

void UTankTrack::DriveTrack()
{	
	FVector ForceApplied = this->GetForwardVector() * this->m_throttleCurrent * this->TrackMaxDrivingForce;
	FVector ForceLocation = this->GetComponentLocation();
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(this->GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString TankName = this->GetOwner()->GetName();

	ApplySidewaysForce();
	DriveTrack();
	//reset throttle current to avoid keeping speed
	this->m_throttleCurrent = 0.0f;
}

void UTankTrack::ApplySidewaysForce()
{
	//Calculate the slippage speed
	//if tank goes to the right cause the physics he value will be more than one
	float SlippageSpeed = FVector::DotProduct(this->GetRightVector(), this->GetComponentVelocity());

	float DeltaTime = this->GetWorld()->GetDeltaSeconds();
	//Work out the required acceleration this frame to correct
	// a = v / t
	FVector CorrectionAcceleration = (-SlippageSpeed / DeltaTime) * this->GetRightVector();
	//Calculate and apply sideways for (F = m a)
	UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(this->GetOwner()->GetRootComponent());
	FVector CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2.0f; //divide by 2 cause 2 tracks
	TankRoot->AddForce(CorrectionForce);
}


