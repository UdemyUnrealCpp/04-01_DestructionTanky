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

	TArray<ATankPlayerController*> GetTankPlayerControllers() const;
	TArray<APawn*> GetPawnsControlledByPlayers() const;

	ATankPlayerController* GetTankPlayerControllers(int32 Index) const;

	


private:
	TArray<ATankPlayerController*> m_TankPlayerControllersArray;

	UPROPERTY(EditAnywhere, Category = Enum)
		EGameState m_GameState;

	UFUNCTION(BlueprintCallable, Category = "PlayerRef")
		void SetTankPlayerControllers(TArray<ATankPlayerController*> TankPlayerControllers);

	UFUNCTION(BlueprintCallable, Category = "GameState")
		void SetGameState(EGameState NewGameState);

};
