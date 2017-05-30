// Copyright Lambot Johan

#include "BattleTank.h"
#include "HoverTankTrack.h"


UHoverTankTrack::UHoverTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHoverTankTrack::BeginPlay()
{
	Super::BeginPlay();

	OnComponentHit.AddDynamic(this, &UHoverTankTrack::OnHit);
}

void UHoverTankTrack::SetThrottleVector(FVector ThrottleVector)
{
	float ThrottleX = FMath::Clamp<float>(ThrottleVector.X, -1.0f, 1.0f);
	float ThrottleY = FMath::Clamp<float>(ThrottleVector.Y, -1.0f, 1.0f);
	float ThrottleZ = 0.0f;

	this->m_ThrottleVectorCurrent = FVector(ThrottleX, ThrottleY, ThrottleZ).GetSafeNormal();

 //UE_LOG(LogTemp, Warning, TEXT("Vector : %s"), *this->m_ThrottleVectorCurrent.ToString());
}

void UHoverTankTrack::DriveTrack(float DeltaTime)
{
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(this->GetOwner()->GetRootComponent());


	FVector ForceApplied = TankRoot->GetForwardVector() * this->m_ThrottleVectorCurrent.X /* * this->TrackMaxDrivingForce*/;
	ForceApplied += TankRoot->GetRightVector() * this->m_ThrottleVectorCurrent.Y /** this->TrackMaxDrivingForce*/;

	//UE_LOG(LogTemp, Warning, TEXT("ForceAppliedX : %s"), *(TankRoot->GetForwardVector() * this->m_ThrottleVectorCurrent.X).ToString());
	//UE_LOG(LogTemp, Warning, TEXT("ForceAppliedY : %s"), *(TankRoot->GetRightVector() * this->m_ThrottleVectorCurrent.Y).ToString());

	FVector ForceLocation = this->GetComponentLocation();
	
	//TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);

	TankRoot->SetWorldLocation(TankRoot->GetComponentLocation() + ForceApplied * DeltaTime * 100.0f);
}

void UHoverTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//FString TankName = this->GetOwner()->GetName();

	//DriveTrack();
	//reset throttle current to avoid keeping speed
	//this->m_throttleCurrent = 0.0f;
}

void UHoverTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Calculate the slippage speed
	//if tank goes to the right cause the physics he value will be more than one
	/*float SlippageSpeed = FVector::DotProduct(this->GetForwardVector(), this->GetComponentVelocity());
	UE_LOG(LogTemp, Warning, TEXT("SlippageSpeed : %f"), SlippageSpeed);

	//Work out the required acceleration this frame to correct
	if (SlippageSpeed < -1.0f)
	{
		// a = v / t
		FVector CorrectionAcceleration = (-SlippageSpeed / DeltaTime) * this->GetForwardVector();
		//UE_LOG(LogTemp, Warning, TEXT("CorrectionAcceleration : %s"), *CorrectionAcceleration.ToString());




		//Calculate and apply sideways for (F = m a)
		UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(this->GetOwner()->GetRootComponent());
		FVector CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration); //divide by 2 cause 2 tracks
		TankRoot->AddForce(CorrectionForce);
	}*/



	//DriveTrack(DeltaTime);
	//reset throttle current to avoid keeping speed
	//this->m_ThrottleVectorCurrent = FVector::ZeroVector;
}
