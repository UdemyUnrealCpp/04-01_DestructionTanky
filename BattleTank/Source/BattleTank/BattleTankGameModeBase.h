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

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
		void GameEnd();
private:

	virtual void Tick(float DeltaSeconds) override;

	TArray<ATankPlayerController*> m_TankPlayerControllersArray;

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

	float m_GamePlayingTimeStart;
	float m_GamePlayingTimeElapsed;

};
