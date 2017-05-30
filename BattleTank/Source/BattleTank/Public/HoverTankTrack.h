// Copyright Lambot Johan

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "HoverTankTrack.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UHoverTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	//Sets a throttle between -1 and +1
	UFUNCTION(BlueprintCallable, Category = "Input")
		void SetThrottleVector(FVector ThrottleVector);

	//max force per track, in Nsewtons;
	UPROPERTY(EditDefaultsOnly, Category = "TankTrack")
		float TrackMaxDrivingForce = 40000000.0; //Assume 40 tonne tank, and 1g acceleration (40000 * 10)

private:
	UHoverTankTrack();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void DriveTrack(float DeltaTime);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FVector m_ThrottleVectorCurrent;
	
	
};
