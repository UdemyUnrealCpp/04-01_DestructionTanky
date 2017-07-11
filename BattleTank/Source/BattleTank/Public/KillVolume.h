// Copyright Lambot Johan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PhysicsVolume.h"
#include "KillVolume.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API AKillVolume : public APhysicsVolume
{
	GENERATED_BODY()

public:
	virtual void ActorEnteredVolume(class AActor* Other) override;
	
};
