#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GribbitOutfitComponent.generated.h"

/**
 * UGribbitOutfitComponent
 * 
 * Handles outfit swapping for Gribbits.
 * Will be expanded to support the iconic looks:
 * - Chill Pete (Pizza hat)
 * - Sheriff Buck (Cowboy)
 * - Max MARFA (Streetwear)
 * - Captain Finn (Sailor)
 * - Elder Moss (Wise)
 * - Lord Tiberius (Tycoon)
 * - Lana MARFA (Brand Ambassador)
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitOutfitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitOutfitComponent();

	UFUNCTION(BlueprintCallable, Category = "Outfit")
	void SetOutfit(FName OutfitID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
	FName CurrentOutfit = "Default";
};