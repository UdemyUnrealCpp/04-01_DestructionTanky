// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Tank.h"
#include "TankPlayerController.h"



void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*m_tankControlled = Cast<ATank>(this->GetPawn());

	if (!ensure(m_tankControlled)) { return; }*/	
	


	this->m_bIsInitialized = false;
}


void ATankPlayerController::Tick(float fDeltaTime)
{
	Super::Tick(fDeltaTime);

	if (!this->m_bIsInitialized)
	{
		CheckInitialization();
	}
	else
	{
		AimTowardsCrosshair();
	}
    
}

void ATankPlayerController::CheckInitialization()
{
	this->m_bIsInitialized = false;
	
	if (m_tankControlled != nullptr)
	{
		Init();		
	}	
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!this->m_bIsInitialized)
		return;

	if (!GetPawn()) { return; } //if not possessing

	if (!ensure(this->m_tankControlled)) { return; }

	UTankAimingComponent* AimComp = this->m_tankControlled->GetTankAimingComponent();
	if (!ensure(AimComp)) { return; }

	FVector LookDirection;
	GetSightRayHitDirection(LookDirection);
	AimComp->AimAtDirection(LookDirection);
}


bool ATankPlayerController::GetSightRayHitDirection(FVector &outHitDirection) const
{
	//find crosshair position in pixel coordinate
	int32 iViewportSizeX, iViewportSizeY;
	GetViewportSize(iViewportSizeX, iViewportSizeY);

	UGameViewportClient* Viewport = GetWorld()->GetGameViewport();
	FSplitscreenData SplitData = Viewport->SplitscreenInfo[Viewport->GetCurrentSplitscreenConfiguration()];
	float PercentageX = SplitData.PlayerData[this->NetPlayerIndex].SizeX;
	float PercentageY = SplitData.PlayerData[this->NetPlayerIndex].SizeY;

	float screenStartX = SplitData.PlayerData[this->NetPlayerIndex].OriginX * iViewportSizeX;
	float screenStartY = SplitData.PlayerData[this->NetPlayerIndex].OriginY * iViewportSizeY;

	float screenSizeX = SplitData.PlayerData[this->NetPlayerIndex].SizeX * iViewportSizeX;
	float screenSizeY = SplitData.PlayerData[this->NetPlayerIndex].SizeY * iViewportSizeY;



	
	UE_LOG(LogTemp, Warning, TEXT("Viewport : %d %d"), iViewportSizeX, iViewportSizeY);

	FVector2D ScreenLocation = FVector2D(iViewportSizeX * this->CrosshairXLocation, iViewportSizeY * this->CrosshairYLocation);
	ScreenLocation = FVector2D(screenStartX + (this->CrosshairXLocation * screenSizeX), screenStartY + (this->CrosshairYLocation * screenSizeY));
	//FIntPoint test = GetLocalPlayer()->ViewportClient->Viewport->GetSizeXY();
	//APlayerState *state = GetSplitscreenPlayerByIndex(this->NetPlayerIndex);
	//UE_LOG(LogTemp, Warning, TEXT("Screen1 : %d"), state->);

	
	

		//UE_LOG(LogTemp, Warning, TEXT("Screen1 : %f"), );
		UE_LOG(LogTemp, Warning, TEXT("Screen1 : %f"), SplitData.PlayerData[this->NetPlayerIndex].OriginX);
	//deproject the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		//line trace along that look direction and see what we hit (up to max range)
		FVector HitLocation;
		if (GetLookVectorHitLocation(LookDirection, HitLocation))
		{
			//get barrel location
			//+ calculate look direction start from barrel and not the camera
			UTankAimingComponent* AimComp = this->GetPawn()->FindComponentByClass<UTankAimingComponent>();
			if (!ensure(AimComp)) { return false; }
			
			FVector StartLocation = AimComp->GetBarrelLocation();
			LookDirection = HitLocation - StartLocation;
			outHitDirection = LookDirection.GetSafeNormal();

			DrawDebugLine(
				GetWorld(),
				StartLocation,
				StartLocation + outHitDirection * 10000000.0f,
				FColor(0, 255, 0),
				false, -1, 0,
				12.333
			);
			return true;
		}
	}

	outHitDirection = LookDirection;
	return false;
}

ATank* ATankPlayerController::GetTankControlled() const
{
	return this->m_tankControlled;
}

//deproject the screen position of the crosshair to a world direction
bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{	
	FVector WorldLocation;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, LookDirection);	
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	//get camera location
	FVector StartLocation = this->PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + (LookDirection * LineTraceRangeCentimeters);
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, this->GetPawn());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, TraceParams))
	{
		HitLocation = HitResult.Location;
		//UE_LOG(LogTemp, Warning, TEXT("HIT : %s"), *HitResult.GetActor()->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("HIT Location : %s"), *HitLocation.ToString());
		return true;
	}

	HitLocation = FVector(0);
	return false;
}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetPawn %s"), *InPawn->GetName());
		ATank* PossessedTank = Cast<ATank>(InPawn);

		if (!ensure(PossessedTank)) { return; }
		//a pawn is given to the player
		//keep a ref
		m_tankControlled = Cast<ATank>(this->GetPawn());
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
	}
}

void ATankPlayerController::OnPossessedTankDeath()
{
	StartSpectatingOnly();
}

void ATankPlayerController::InputBoost()
{
	if (!this->m_bIsInitialized)
		return;

	if (!ensure(this->m_tankControlled))
		return;

	this->m_tankControlled->LaunchBoost();
}

void ATankPlayerController::InputFire()
{
	if (!this->m_bIsInitialized)
		return;

	if (!ensure(this->m_tankControlled))
		return;

	this->m_tankControlled->Fire();
}

void ATankPlayerController::InputMove(float ForwardAxisValue, float RightAxisValue)
{
	if (!this->m_bIsInitialized)
		return;

	if (!ensure(this->m_tankControlled))
		return;

	this->m_tankControlled->Move(ForwardAxisValue, RightAxisValue);
}

void ATankPlayerController::Init()
{
	InitPlayerUIWidget(m_tankControlled);

	this->m_bIsInitialized = true;
}




