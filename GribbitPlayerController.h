#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GribbitPlayerController.generated.h"

class UGribbitNeedsWidget;
class AGribbitCharacter;

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
};
