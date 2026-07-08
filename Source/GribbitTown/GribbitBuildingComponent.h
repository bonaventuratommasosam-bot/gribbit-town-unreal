#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GribbitBuildingComponent.generated.h"

/**
 * UGribbitBuildingComponent
 * 
 * Sistema di costruzione base per Gribbits Town (stile Mini World).
 * Supporta piazzamento e rimozione oggetti.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitBuildingComponent();

	UFUNCTION(BlueprintCallable, Category = "Building")
	bool PlaceObjectInFront(TSubclassOf<AActor> ObjectClass, float Distance = 200.f);

	UFUNCTION(BlueprintCallable, Category = "Building")
	bool RemoveObjectInFront(float Distance = 250.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSubclassOf<AActor> DefaultPlaceableObject;
};