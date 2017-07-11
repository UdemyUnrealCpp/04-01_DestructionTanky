// Copyright Lambot Johan

#include "BattleTank.h"

#include "Tank.h"

#include "KillVolume.h"


void AKillVolume::ActorEnteredVolume(class AActor* Other)
{
	Super::ActorEnteredVolume(Other);

	if (Other == nullptr)
		return;

	UE_LOG(LogTemp, Warning, TEXT("VOLUME %s"), *Other->GetName());

	ATank *TankCast = Cast<ATank>(Other);
	if (TankCast == nullptr)
		return;

	UE_LOG(LogTemp, Warning, TEXT("VOLUME TANK %s"), *TankCast->GetName());

	TankCast->AddDamage(9999.0f);
}

