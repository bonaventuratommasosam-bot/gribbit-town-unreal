#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GribbitTownGameMode.generated.h"

class AGribbitCharacter;
class AGribbitPlayerController;
class AGribbitPlayerState;
class AGribbitVehicle;

/**
 * AGribbitTownGameMode
 *
 * Main GameMode for Gribbits Town.
 *
 * Responsibilities:
 * - Sets the default pawn / player controller / player state classes
 *   (multiplayer-ready; Feature 5).
 * - Spawns the townsfolk (the 7 iconic frogs) from DT_Characters, around
 *   whichever PlayerStart the current map provides, so the same GameMode
 *   works in GribbitCity and in GribbitTown_Main.
 *
 * Which map is played is decided by the launcher or by GameDefaultMap in
 * Config/DefaultEngine.ini -- the GameMode no longer redirects to a map of
 * its own, because that made every other map unplayable.
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

	// Number of AI townsfolk to spawn from DT_Characters.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	int32 TownfolkCount = 6;

	// How far from the player start the townsfolk are scattered, in cm.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	float TownfolkSpread = 700.f;

	// Spawns the townsfolk from DT_Characters around the player start.
	UFUNCTION(BlueprintCallable, Category="Gribbit")
	void SpawnTownfolk();

	/**
	 * Turns the nearest parked scenery cars into drivable ones.
	 *
	 * build_city.py fills the kerbs with plain StaticMeshActors, which are
	 * cheap and look right but cannot be driven. At BeginPlay we swap the few
	 * closest to the player for real AGribbitVehicle pawns, same spot and
	 * angle. Done here rather than in the map because the headless editor
	 * crashes on spawning a pawn with a camera, and because the generated
	 * .umap is not versioned -- this way a fresh clone gets its cars too.
	 */
	UFUNCTION(BlueprintCallable, Category="Gribbit|Vehicle")
	void SpawnDrivableCars();

	// Blueprint cars to draw from, in order. Filled with /Game/Vehicles/BP_Car_*.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit|Vehicle")
	TArray<TSubclassOf<AGribbitVehicle>> DrivableCarClasses;

	// How many parked cars to promote. 0 disables the swap.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit|Vehicle")
	int32 DrivableCarCount = 5;

	// Only cars within this distance of the player start are considered, in cm.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit|Vehicle")
	float DrivableCarSearchRadius = 8000.f;

protected:
	virtual void BeginPlay() override;
};
