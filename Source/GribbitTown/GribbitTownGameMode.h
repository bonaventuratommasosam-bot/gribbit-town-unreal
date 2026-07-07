#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GribbitTownGameMode.generated.h"

/**
 * AGribbitTownGameMode
 * 
 * Main GameMode for Gribbits Town.
 * 
 * Current responsibilities:
 * - Spawn the player-controlled Gribbit (AGribbitCharacter)
 * - Procedurally generate a basic town layout at StartPlay (for quick prototyping)
 * 
 * Future direction (aligned with design):
 * - Move toward manual level design + World Partition / Level Streaming
 * - Initialize core systems: Needs, Character Manager, Town Manager
 * - Support for the 7 iconic characters (Chill Pete, Sheriff Buck, Max MARFA, etc.)
 * - Prepare foundation for online/multiplayer features
 */
UCLASS()
class GRIBBITTOWN_API AGribbitTownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGribbitTownGameMode();

	virtual void StartPlay() override;

	// TODO: Add references to core managers once created
	// UPROPERTY(EditDefaultsOnly, Category = "Gribbits Town")
	// TSubclassOf<class UGribbitNeedsManager> NeedsManagerClass;
};