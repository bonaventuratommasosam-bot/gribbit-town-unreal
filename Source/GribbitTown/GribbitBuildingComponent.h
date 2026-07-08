#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GribbitBuildingComponent.generated.h"

/**
 * UGribbitBuildingComponent
 * 
 * Componente base per il sistema di costruzione stile Mini World / sandbox.
 * Permette al Gribbit di piazzare oggetti, blocchi e strutture.
 * 
 * Questa è la base per il gameplay sandbox online.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitBuildingComponent();

	UFUNCTION(BlueprintCallable, Category = "Building")
	bool TryPlaceObject(TSubclassOf<AActor> ObjectClass, const FTransform& Transform);

	// TODO: Aggiungere sistema di preview, snapping, rimozione oggetti, salvataggio
};