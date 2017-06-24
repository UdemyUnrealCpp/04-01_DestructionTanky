// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "EngineUtils.h"
#include "TankAIController.h"
#include "Tank.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float fDeltaTime)
{
	Super::Tick(fDeltaTime);

	/*
	APawn* ControlledTank = this->GetPawn();
	APawn* PlayerTank = this->GetPlayerTank();

	if (!ensure(ControlledTank && PlayerTank))
		return;

	MoveToActor(PlayerTank, AcceptanceRadius);

	//ControlledTank->AimAt(Target->GetActorLocation());
	UTankAimingComponent *AimComp = this->GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimComp != nullptr)) { return; }

	AimComp->AimAtLocation(PlayerTank->GetActorLocation());

	if(AimComp->GetFiringState() == EFiringState::EFiringStatus_LOCKED)
		AimComp->Fire();*/
}

APawn* ATankAIController::GetPlayerTank() const
{
	return this->GetWorld()->GetFirstPlayerController()->GetPawn();
	//TODO need to find a solution to check if pawn is a tank without use cast <ATank>
	/*
	//loop through all APlayerController for the world
	for (TActorIterator<APlayerController> iterator(GetWorld()); iterator; ++iterator)
	{
		//search APlayerController (which is a parent of ATankPlayerController)
		ATank *playerTank = Cast<ATank>(iterator->GetPawn());

		if (playerTank)
		{
			//UE_LOG(LogTemp, Warning, TEXT("FOUND PLAYER CONTROLLER %s"), *(playerTank->GetName()));
			//return first player tank found
			return iterator->GetPawn();
		}
	}
	*/

	return nullptr;
}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		ATank* PossessedTank = Cast<ATank>(InPawn);

		if (!ensure(PossessedTank)) { return; }
		
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!this->GetPawn()) { return; }

	this->GetPawn()->DetachFromControllerPendingDestroy();
}
