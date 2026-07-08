#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GribbitInteractionComponent.generated.h"

/**
 * UGribbitInteractionComponent
 * 
 * Gestisce le interazioni del Gribbit con il mondo (oggetti, NPC, ecc).
 * Versione base per rendere il gioco più giocabile.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitInteractionComponent();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TryInteract();

	// Raggio di interazione
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionRange = 250.f;
};