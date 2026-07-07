#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GribbitInteractable.generated.h"

class AGribbitCharacter;

/**
 * AGribbitInteractable
 *
 * Base class for anything a frog can interact with (pizza oven, bench,
 * quest giver, door...). The interaction component on the player traces
 * to these actors and calls OnInteract.
 */
UCLASS(Abstract, Blueprintable)
class GRIBBITTOWN_API AGribbitInteractable : public AActor
{
	GENERATED_BODY()

public:
	AGribbitInteractable();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Text shown in the interaction prompt when focused.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FText InteractionPrompt = NSLOCTEXT("Gribbit", "Interact", "Interact");

	// Max distance at which this can be focused.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float InteractionRange = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bEnabled = true;

	// Called on the server when a character interacts.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	void OnInteract(AGribbitCharacter* Interactor);
	virtual void OnInteract_Implementation(AGribbitCharacter* Interactor);

	// Whether this interactable currently accepts interaction.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	bool CanInteract(AGribbitCharacter* Interactor) const;
	virtual bool CanInteract_Implementation(AGribbitCharacter* Interactor) const;
};
