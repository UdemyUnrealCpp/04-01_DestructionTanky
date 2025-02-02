// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float CrosshairXLocation = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)		
		float CrosshairYLocation = 1.0f / 3.0f;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 CrosshairXSize = 64;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 CrosshairYSize = 64;


	UFUNCTION(BlueprintCallable, Category = "Input")
		void InputBoost();

	UFUNCTION(BlueprintCallable, Category = "Input")
		void InputFire();

	UFUNCTION(BlueprintCallable, Category = "Input")
		void InputMove(float ForwardAxisValue, float RightAxisValue);

	UFUNCTION(BlueprintCallable, Category = "TankRef")
		ATank* GetTankControlled() const;

	int32 GetLeaderboardPosition() const;
	float GetSurviveTime() const;
	bool IsDead() const;

	
protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void InitPlayerUIWidget(ATank* TankPlayerRef);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dead")
		void Dead();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float fDeltaTime) override;	
	void CheckInitialization();


	//Start the tank moving the barrel so that a shot would hit where
	//the crosshair intersects the world
	void AimTowardsCrosshair();

	//return OUT parameter, true if hit landscape
	bool GetSightRayHitDirection(FVector &outHitDirection) const;
	//deproject the screen position of the crosshair to a world direction
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;	
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	UPROPERTY(EditAnywhere)
	int32 LineTraceRangeCentimeters = 1000000;

	virtual void SetPawn(APawn *InPawn) override;
	void Init();
	void InitSplitScreenData();

	UFUNCTION()
		void OnPossessedTankDeath();

	ATank* m_tankControlled;

	bool m_bIsInitialized;

	bool m_bIsDead;

	FVector2D m_splitscreenOrigin;
	FVector2D m_splitscreenSize;

	float m_SurvivreTime;
	int32 m_LeaderboardPosition;
};

