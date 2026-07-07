#include "GribbitInteractable.h"
#include "GribbitCharacter.h"
#include "Net/UnrealNetwork.h"

AGribbitInteractable::AGribbitInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
}

void AGribbitInteractable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGribbitInteractable, InteractionPrompt);
}

void AGribbitInteractable::OnInteract_Implementation(AGribbitCharacter* Interactor)
{
	// Base: no-op. Blueprints / subclasses implement the effect.
}

bool AGribbitInteractable::CanInteract_Implementation(AGribbitCharacter* Interactor) const
{
	return bEnabled;
}
