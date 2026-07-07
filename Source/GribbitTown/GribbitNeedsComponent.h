#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GribbitNeedsComponent.generated.h"

/**
 * UGribbitNeedsComponent
 *
 * Manages the core needs of a Gribbit (frog character).
 * Based on our design: Hunger, Energy, Fun, Social, Hygiene, Bladder.
 * 
 * This is the foundation for the Sims-like needs system in Gribbits Town.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitNeedsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitNeedsComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Needs (0-100 range)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Hunger = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Energy = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Fun = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Social = 60.f;

	// TODO: Add more needs (Hygiene, Bladder) and decay rates

	UFUNCTION(BlueprintCallable, Category = "Needs")
	void ModifyNeed(FName NeedName, float Amount);
};