// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "BattleTankGameModeBase.generated.h"


class ATankPlayerController;
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

	UFUNCTION(BlueprintCallable, Category = "PlayerRef")
		void SetTankPlayerControllers(TArray<ATankPlayerController*> TankPlayerControllers);
};
