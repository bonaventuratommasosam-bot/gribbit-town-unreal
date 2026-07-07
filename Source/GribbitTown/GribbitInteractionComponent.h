#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GribbitInteractionComponent.generated.h"

class AGribbitInteractable;
class AGribbitCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusChanged, AGribbitInteractable*, NewFocus);

/**
 * UGribbitInteractionComponent
 *
 * Attached to the player character. Each tick (client) it traces forward from
 * the camera and focuses the nearest valid AGribbitInteractable within range.
 * Pressing Interact (server) calls OnInteract on the focused actor.
 *
 * Feature 4 (simple interaction system).
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitInteractionComponent();

	// Trace distance when looking for interactables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float TraceDistance = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float TraceRadius = 30.f;

	// Currently focused interactable (null if none). Replicated to owning client.
	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	TObjectPtr<AGribbitInteractable> CurrentFocus;

	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnFocusChanged OnFocusChanged;

	// Call from input: attempt to interact with the current focus.
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void TryInteract();

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UpdateFocus();
};
