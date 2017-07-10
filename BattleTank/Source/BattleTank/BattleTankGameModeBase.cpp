// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"

#include "TankPlayerController.h"

#include "BattleTankGameModeBase.h"


ABattleTankGameModeBase::ABattleTankGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}


void ABattleTankGameModeBase::SetTankPlayerControllers(TArray<ATankPlayerController*> TankPlayerControllers)
{
	this->m_TankPlayerControllersArray = TankPlayerControllers;

	for (int i = 0; i < this->m_TankPlayerControllersArray.Num(); ++i)
	{
		if (this->m_TankPlayerControllersArray[i] != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("TPC : %s"), *this->m_TankPlayerControllersArray[i]->GetName());
			if (m_PlayerScore.Num() < this->m_TankPlayerControllersArray.Num())
			{
				//m_PlayerScore.Add(new FBattleTankPlayerScore(i));
				m_PlayerScore.Add(FBattleTankPlayerScore(i));
				UE_LOG(LogTemp, Warning, TEXT("TPCscore : %d"), m_PlayerScore[i].m_PlayerId);
			}		
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
		if (!this->m_TankPlayerControllersArray[i]->IsDead())
		{
			PawnsControlledByPlayers.Add(this->m_TankPlayerControllersArray[i]->GetPawn());
		}
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

int32 ABattleTankGameModeBase::GetNumberPlayerControllerAlive() const
{
	int32 NumberPlayerAlive = 0;

	for (int i = 0; i < this->m_TankPlayerControllersArray.Num(); ++i)
	{
		if (!this->m_TankPlayerControllersArray[i]->IsDead())
		{
			++NumberPlayerAlive;
		}
	}

	return NumberPlayerAlive;
}

float ABattleTankGameModeBase::GetGamePlayingTimeElapsed() const
{
	return this->m_GamePlayingTimeElapsed;
}

void ABattleTankGameModeBase::CheckGameEnd()
{
	int32 NumberPlayerAlive = 0;

	for (int i = 0; i < this->m_TankPlayerControllersArray.Num(); ++i)
	{
		if (!this->m_TankPlayerControllersArray[i]->IsDead())
		{
			++NumberPlayerAlive;
		}
	}

	if (NumberPlayerAlive <= 1)
	{
		SetGameState(EGameState::EGameState_END);	
	}
}


TArray<FBattleTankPlayerScore> ABattleTankGameModeBase::GetPlayerScore() const
{
	return this->m_PlayerScore;
}

EGameState ABattleTankGameModeBase::GetGameState() const
{
	return this->m_GameState;
}


void ABattleTankGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateGameState(DeltaSeconds);	
}

void ABattleTankGameModeBase::SetGameState(EGameState NewGameState)
{

	this->m_GameState = NewGameState;

	switch (this->m_GameState)
	{
	case EGameState::EGameState_INIT:
		{
			UE_LOG(LogTemp, Warning, TEXT("Change state TO EGameState::EGameState_INIT "));
			break;
		}
	case EGameState::EGameState_READY:
		{
			UE_LOG(LogTemp, Warning, TEXT("Change state TO EGameState::EGameState_READY "));

			for (int i = 0; i < this->m_TankPlayerControllersArray.Num(); ++i)
			{
				this->m_TankPlayerControllersArray[i]->DisableInput(this->m_TankPlayerControllersArray[i]);
			}

			this->m_StartTimerCurrent = this->m_StartTimerMax + 1.0f;
			break;
		}
	case EGameState::EGameState_PLAYING:
		{
			UE_LOG(LogTemp, Warning, TEXT("Change state TO EGameState::EGameState_PLAYING "));

			for (int i = 0; i < this->m_TankPlayerControllersArray.Num(); ++i)
			{
				this->m_TankPlayerControllersArray[i]->EnableInput(this->m_TankPlayerControllersArray[i]);
			}

			m_GamePlayingTimeStart = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			break;
		}
	case EGameState::EGameState_END:
		{
			UE_LOG(LogTemp, Warning, TEXT("Change state TO EGameState::EGameState_END "));		

			for (int i = 0; i < this->m_TankPlayerControllersArray.Num(); ++i)
			{
				this->m_TankPlayerControllersArray[i]->DisableInput(this->m_TankPlayerControllersArray[i]);
				CalculatePlayersScore(i, this->m_TankPlayerControllersArray[i]->IsDead());
			}

			GameEnd();
			break;
		}
	}
}

float ABattleTankGameModeBase::GetStartTimerCurrent()
{
	return this->m_StartTimerCurrent;
}


void ABattleTankGameModeBase::UpdateGameState(float DeltaSeconds)
{
	switch (this->m_GameState)
	{
	case EGameState::EGameState_INIT:
	{
		break;
	}
	case EGameState::EGameState_READY:
	{
		this->m_StartTimerCurrent -= DeltaSeconds;

		if (this->m_StartTimerCurrent <= 1.0f)//1.0f for the display of GO in ui
		{
			this->SetGameState(EGameState::EGameState_PLAYING);
		}
		break;
	}
	case EGameState::EGameState_PLAYING:
	{
		if (this->m_StartTimerCurrent > 0.0f)
		{
			this->m_StartTimerCurrent -= DeltaSeconds;
		}

		m_GamePlayingTimeElapsed = UGameplayStatics::GetRealTimeSeconds(GetWorld()) - m_GamePlayingTimeStart;
		//UE_LOG(LogTemp, Warning, TEXT("%s __ %f"), *this->GetName(), m_GamePlayingTimeElapsed);
		break;
	}
	}
}

void ABattleTankGameModeBase::CalculatePlayersScore(int32 PlayerId, bool isDeath)
{
	if (isDeath)
	{
		this->m_PlayerScore[PlayerId].m_PlayerLeaderboardPos = this->m_TankPlayerControllersArray[PlayerId]->GetLeaderboardPosition();
		this->m_PlayerScore[PlayerId].m_PlayerScore = this->m_TankPlayerControllersArray.Num() - this->m_PlayerScore[PlayerId].m_PlayerLeaderboardPos;
		this->m_PlayerScore[PlayerId].m_SurvivreTime = this->m_TankPlayerControllersArray[PlayerId]->GetSurviveTime();
	}
	else // first
	{
		this->m_PlayerScore[PlayerId].m_PlayerLeaderboardPos = 1;
		this->m_PlayerScore[PlayerId].m_PlayerScore = this->m_TankPlayerControllersArray.Num() - this->m_PlayerScore[PlayerId].m_PlayerLeaderboardPos;
		this->m_PlayerScore[PlayerId].m_SurvivreTime = 999999;
	}
}
