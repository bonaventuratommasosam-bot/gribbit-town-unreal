#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GribbitBuildingComponent.generated.h"

/**
 * UGribbitBuildingComponent
 * 
 * Sistema di costruzione base stile Mini World / sandbox.
 * Permette di piazzare oggetti semplici.
 * Versione iniziale per rendere il gioco giocabile il prima possibile.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitBuildingComponent();

	/** Prova a piazzare un oggetto di fronte al giocatore */
	UFUNCTION(BlueprintCallable, Category = "Building")
	bool PlaceObjectInFront(TSubclassOf<AActor> ObjectClass, float Distance = 200.f);

	/** Oggetto di esempio da piazzare (puoi cambiarlo in Blueprint) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSubclassOf<AActor> DefaultPlaceableObject;
};