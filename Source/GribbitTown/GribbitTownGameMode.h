#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GribbitTownGameMode.generated.h"

/**
 * Default game mode for Gribbit Town.
 * Spawns the Gribbit character as the player pawn and
 * procedurally builds the town (ground, buildings, trees, frogs) on StartPlay.
 */
UCLASS()
class GRIBBITTOWN_API AGribbitTownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGribbitTownGameMode();

	virtual void StartPlay() override;
};
