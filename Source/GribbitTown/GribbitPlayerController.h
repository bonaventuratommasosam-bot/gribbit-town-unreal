#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GribbitPlayerController.generated.h"

class UGribbitNeedsWidget;
class AGribbitCharacter;
class AGribbitVehicle;

/**
 * AGribbitPlayerController
 *
 * Feature 5 (multiplayer) + HUD owner. Creates the needs HUD widget for the
 * local player and routes the Interact input to the character's interaction
 * component.
 */
UCLASS()
class GRIBBITTOWN_API AGribbitPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGribbitPlayerController();

	// Widget Blueprint class for the needs HUD (set in editor, or via C++ default).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UGribbitNeedsWidget> NeedsWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	TObjectPtr<UGribbitNeedsWidget> NeedsWidget;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Build (or rebuild) the needs HUD for the local player.
	UFUNCTION(BlueprintCallable, Category="UI")
	void CreateNeedsHUD();

	void HandleInteractInput();

	/**
	 * Gets the frog into the nearest car. Cars are pawns, not
	 * AGribbitInteractable actors, so they sit outside the look-at interaction
	 * system: this is a straight proximity check on its own key.
	 */
	void HandleEnterVehicleInput();

	/** How far from the frog a car can be and still be boardable, in cm. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit|Vehicle")
	float VehicleBoardingRange = 400.f;
};
