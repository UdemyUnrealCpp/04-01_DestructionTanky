// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "BattleTankGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ABattleTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	
	
		UPROPERTY(EditDefaultsOnly)
		float HeightElevation = 250.0f;
};
