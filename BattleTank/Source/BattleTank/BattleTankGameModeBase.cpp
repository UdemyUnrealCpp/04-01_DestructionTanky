// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"

#include "TankPlayerController.h"

#include "BattleTankGameModeBase.h"



void ABattleTankGameModeBase::SetTankPlayerControllers(TArray<ATankPlayerController*> TankPlayerControllers)
{
	this->m_TankPlayerControllersArray = TankPlayerControllers;

	for (int i = 0; i < this->m_TankPlayerControllersArray.Num(); ++i)
	{
		if (this->m_TankPlayerControllersArray[i] != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("TPC : %s"), *this->m_TankPlayerControllersArray[i]->GetName());
		}
	}
}


TArray<ATankPlayerController*> ABattleTankGameModeBase::GetTankPlayerControllers() const
{
	return this->m_TankPlayerControllersArray;
}


TArray<APawn*> ABattleTankGameModeBase::GetPawnsControlledByPlayers() const
{
	TArray<APawn*> PawnsControlledByPlayers;

	for (int i = 0; i < this->m_TankPlayerControllersArray.Num(); ++i)
	{
		PawnsControlledByPlayers.Add(this->m_TankPlayerControllersArray[i]->GetPawn());
	}

	return PawnsControlledByPlayers;
}

ATankPlayerController* ABattleTankGameModeBase::GetTankPlayerControllers(int32 Index) const
{
	if (Index >= 0 && Index < this->m_TankPlayerControllersArray.Num())
	{
		return this->m_TankPlayerControllersArray[Index];
	}

	return nullptr;
}


