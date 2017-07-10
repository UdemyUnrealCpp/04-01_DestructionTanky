// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "BattleTankGameModeBase.generated.h"


class ATankPlayerController;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGameState : uint8
{
	EGameState_INIT UMETA(DisplayName = "Init"),
	EGameState_READY UMETA(DisplayName = "Ready"),
	EGameState_PLAYING	UMETA(DisplayName = "Playing"),
	EGameState_END	UMETA(DisplayName = "End")
};

USTRUCT(BlueprintType)
struct FBattleTankPlayerScore
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
		int32 m_PlayerId;

	//for current game
	UPROPERTY(BlueprintReadOnly)
		int32 m_PlayerLeaderboardPos;

	UPROPERTY(BlueprintReadOnly)
		int32 m_PlayerScore;

	UPROPERTY(BlueprintReadOnly)
		float m_SurvivreTime;


	FBattleTankPlayerScore()
	{
		m_PlayerId = -1;
		m_PlayerLeaderboardPos = -1;
		m_PlayerScore = -1;
		m_SurvivreTime = 0.0f;
	}

	FBattleTankPlayerScore(int PlayerId)
	{
		m_PlayerId = PlayerId;
		m_PlayerLeaderboardPos = -1;
		m_PlayerScore = -1;
		m_SurvivreTime = 0.0f;
	}
};


/**
 * 
 */
UCLASS()
class BATTLETANK_API ABattleTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ABattleTankGameModeBase();

	TArray<ATankPlayerController*> GetTankPlayerControllers() const;
	TArray<APawn*> GetPawnsControlledByPlayers() const;

	ATankPlayerController* GetTankPlayerControllers(int32 Index) const;

	int32 GetNumberPlayerControllerAlive() const;

	float GetGamePlayingTimeElapsed() const;

	void CheckGameEnd();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	TArray<FBattleTankPlayerScore> GetPlayerScore() const;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
		void GameEnd();
private:

	virtual void Tick(float DeltaSeconds) override;

	TArray<ATankPlayerController*> m_TankPlayerControllersArray;

	TArray<FBattleTankPlayerScore> m_PlayerScore;

	UPROPERTY(EditAnywhere, Category = "Enum")
		EGameState m_GameState;

	UPROPERTY(EditAnywhere, Category = "Timer")
	float m_StartTimerMax = 3.0f;
	float m_StartTimerCurrent;

	UFUNCTION(BlueprintCallable, Category = "PlayerRef")
		void SetTankPlayerControllers(TArray<ATankPlayerController*> TankPlayerControllers);

	UFUNCTION(BlueprintCallable, Category = "GameState")
		void SetGameState(EGameState NewGameState);

	UFUNCTION(BlueprintCallable, Category = "Timer")
		float GetStartTimerCurrent();


	void UpdateGameState(float DeltaSeconds);
	void CalculatePlayersScore(int32 PlayerId, bool isDeath);

	float m_GamePlayingTimeStart;
	float m_GamePlayingTimeElapsed;

};




