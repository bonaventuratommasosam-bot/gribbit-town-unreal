#pragma once

#include "CoreMinimal.h"
#include "GribbitInteractable.h"
#include "GribbitConsumable.generated.h"

/**
 * AGribbitConsumable
 *
 * A simple interactable that restores one or more needs when a frog
 * interacts with it (Feature 4 in action). Example: a pizza oven that
 * fills Hunger. Place instances in the level and set the NeedAmounts map.
 */
UCLASS(Blueprintable)
class GRIBBITTOWN_API AGribbitConsumable : public AGribbitInteractable
{
	GENERATED_BODY()

public:
	AGribbitConsumable();

	// Need name -> amount restored on interact (e.g. "Hunger" -> 40).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TMap<FName, float> NeedAmounts;

	// If true, the item is consumed (destroyed) after one use.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bConsumeOnUse = false;

	virtual void OnInteract_Implementation(AGribbitCharacter* Interactor) override;
};
