// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "BattleTankGameModeBase.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Tank.h"
#include "TankPlayerController.h"



void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*m_tankControlled = Cast<ATank>(this->GetPawn());

	if (!ensure(m_tankControlled)) { return; }*/	

	//this->InitSplitScreenData();
	this->m_bIsInitialized = false;
	this->m_bIsDead = false;
}


void ATankPlayerController::Tick(float fDeltaTime)
{
	Super::Tick(fDeltaTime);

	if (this->m_bIsDead)
		return;


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
		InitSplitScreenData();		
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
	

	//get screen size
	int32 iViewportSizeX, iViewportSizeY;
	GetViewportSize(iViewportSizeX, iViewportSizeY);

	//find crosshair position in pixel coordinate
	FVector2D ScreenLocation = FVector2D(m_splitscreenOrigin.X + ((this->CrosshairXLocation * m_splitscreenSize.X) /*+ (this->CrosshairXSize / 2)*/), m_splitscreenOrigin.Y + ((this->CrosshairYLocation * m_splitscreenSize.Y) /*+ (this->CrosshairYSize / 2)*/));

	//UE_LOG(LogTemp, Warning, TEXT("ScreenLocation %s"), *ScreenLocation.ToString());

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

			/*
			DrawDebugLine(
				GetWorld(),
				StartLocation,
				StartLocation + outHitDirection * 10000000.0f,
				FColor(0, 255, 0),
				false, -1, 0,
				12.333
			);*/
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

int32 ATankPlayerController::GetLeaderboardPosition() const
{
	return this->m_LeaderboardPosition;
}

float ATankPlayerController::GetSurviveTime() const
{
	return this->m_SurvivreTime;
}

bool ATankPlayerController::IsDead() const
{
	return this->m_bIsDead;
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
		m_tankControlled = PossessedTank;
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
	}
}

void ATankPlayerController::OnPossessedTankDeath()
{
	ABattleTankGameModeBase* BattleTankGameMode = Cast<ABattleTankGameModeBase>(this->GetWorld()->GetAuthGameMode());
	//include the current player that will be dead
	this->m_LeaderboardPosition = BattleTankGameMode->GetNumberPlayerControllerAlive();
	UE_LOG(LogTemp, Warning, TEXT("m_LeaderboardPosition %d"), this->m_LeaderboardPosition);
	this->m_SurvivreTime = BattleTankGameMode->GetGamePlayingTimeElapsed();
	UE_LOG(LogTemp, Warning, TEXT("m_SurvivreTime %f"), this->m_SurvivreTime);

	StartSpectatingOnly();
	Dead();
	this->DisableInput(this);
	m_tankControlled->Destroy();
	m_bIsDead = true;

	BattleTankGameMode->CheckGameEnd();
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

void ATankPlayerController::InitSplitScreenData()
{
	//get screen complete size
	int32 iViewportSizeX, iViewportSizeY;
	GetViewportSize(iViewportSizeX, iViewportSizeY);

	//get viewport
	UGameViewportClient* Viewport = GetWorld()->GetGameViewport();
	//get all splitdata
	FSplitscreenData SplitData = Viewport->SplitscreenInfo[Viewport->GetCurrentSplitscreenConfiguration()];
	//the player index for multiplayer
	int PlayerIndex = this->NetPlayerIndex;

	//calculate origin of the splitscreen for the player
	m_splitscreenOrigin = FVector2D(SplitData.PlayerData[PlayerIndex].OriginX * iViewportSizeX, SplitData.PlayerData[PlayerIndex].OriginY * iViewportSizeY);
	//calculate size of the splitscreen for the player
	m_splitscreenSize = FVector2D(SplitData.PlayerData[PlayerIndex].SizeX * iViewportSizeX, SplitData.PlayerData[PlayerIndex].SizeY * iViewportSizeY);

	//FVector2D test = FVector2D::ZeroVector;
	//Viewport->GetViewportSize(test);
	//UE_LOG(LogTemp, Warning, TEXT("VP 1 : %s"), *test.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("VP Final : O_ %s   S_ %s"), *m_splitscreenOrigin.ToString(), *m_splitscreenSize.ToString());
}



