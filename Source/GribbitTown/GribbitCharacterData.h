#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GribbitCharacterData.generated.h"

/**
 * FGribbitCharacterRow
 *
 * Data-driven definition of one frog character (Feature 1 + 3). The 7 iconic
 * characters are rows in DT_Characters; the Python setup script reads these
 * rows to spawn the 7 Blueprint children of AGribbitCharacter.
 */
USTRUCT(BlueprintType)
struct GRIBBITTOWN_API FGribbitCharacterRow : public FTableRowBase
{
	GENERATED_BODY()

	// Display name shown above the frog.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	FText DisplayName;

	// Short personality blurb.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	FText Bio;

	// Default outfit ID passed to the Outfit component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	FName DefaultOutfit = "Default";

	// Body tint (multiplies the frog material). Optional.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	FLinearColor BodyTint = FLinearColor::White;

	// Starting need values (0..100). Optional override of the component defaults.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float StartHunger = 80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float StartEnergy = 80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float StartFun = 70.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float StartSocial = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float StartHygiene = 75.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float StartBladder = 85.f;
};
