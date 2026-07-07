#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GribbitTownGameMode.generated.h"

class AGribbitCharacter;
class AGribbitPlayerController;
class AGribbitPlayerState;

/**
 * AGribbitTownGameMode
 *
 * Main GameMode for Gribbits Town.
 *
 * Responsibilities:
 * - Sets the default pawn / player controller / player state classes
 *   (multiplayer-ready; Feature 5).
 * - Loads the hand-authored level (GribbitTown_Main) instead of the old
 *   procedural generation (Feature 3).
 * - Spawns the townsfolk (the 7 iconic frogs) from DT_Characters.
 */
UCLASS()
class GRIBBITTOWN_API AGribbitTownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGribbitTownGameMode();

	// Default pawn (BP_ChillPete or the chosen frog), set in the editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	TSubclassOf<AGribbitCharacter> DefaultFrogClass;

	// Player controller / state classes for multiplayer.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	TSubclassOf<AGribbitPlayerController> DefaultFrogController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	TSubclassOf<AGribbitPlayerState> DefaultFrogState;

	// Map to load on start (replaces the old procedural generation).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	FString DefaultMap = TEXT("/Game/Maps/GribbitTown_Main");

	// Number of AI townsfolk to spawn from DT_Characters.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	int32 TownfolkCount = 6;

	// Spawns the townsfolk from DT_Characters at level-design spawn points.
	UFUNCTION(BlueprintCallable, Category="Gribbit")
	void SpawnTownfolk();

protected:
	virtual void BeginPlay() override;
};
